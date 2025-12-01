#include "cura-formulae-engine/ast/ast.h"
#include "cura-formulae-engine/ast/expr_ptr.h"
#include "cura-formulae-engine/eval.h"
#include "cura-formulae-engine/ast/slice_expr.h"

#include <zeus/expected.hpp>

#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace CuraFormulaeEngine::ast
{

[[nodiscard]] std::string SliceExpr::toString() const noexcept
{
    const auto start_index_str = start_index.has_value() ? start_index.value().toString() : "";
    const auto end_index_str = end_index.has_value() ? end_index.value().toString() : "";
    const auto step_size_str = step_size.has_value() ? step_size.value().toString() : "";
    return fmt::format("{}[{}:{}:{}]", array.toString(), start_index_str, end_index_str, step_size_str);
}

[[nodiscard]] eval::Result SliceExpr::evaluate(const env::Environment* environment) const noexcept
{
    std::int64_t step_size_value = int64_t(1);
    if (step_size.has_value())
    {
        const auto end_index_result = try_get<std::int64_t>(step_size.value().evaluate(environment));
        if (! end_index_result.has_value())
        {
            return zeus::unexpected(end_index_result.error());
        }
        step_size_value = end_index_result.value();

        if (step_size_value == int64_t(0))
        {
            return zeus::unexpected(eval::Error::ValueError);
        }
    }

    const auto array_result = try_get<std::vector<eval::Value>>(array.evaluate(environment));
    if (! array_result.has_value())
    {
        return zeus::unexpected(array_result.error());
    }
    const auto& array_value = array_result.value();

    std::int64_t start_index_absolute_value = step_size_value > 0 ? 0 : int64_t(array_value.size()) - 1;
    if (start_index.has_value())
    {
        const auto start_index_result = try_get<std::int64_t>(start_index.value().evaluate(environment));
        if (! start_index_result.has_value())
        {
            return zeus::unexpected(start_index_result.error());
        }
        const auto start_index_value = start_index_result.value();

        if (start_index_value < int64_t(0))
        {
            start_index_absolute_value = int64_t(array_value.size()) - -start_index_value - int64_t(1);
            start_index_absolute_value = std::max(start_index_absolute_value, int64_t(0));
        }
        else
        {
            start_index_absolute_value = start_index_value;
            start_index_absolute_value = std::min(start_index_absolute_value, static_cast<std::int64_t>(array_value.size()) - int64_t(1));
        }
        start_index_absolute_value = std::min(start_index_absolute_value, static_cast<std::int64_t>(array_value.size()) - int64_t(1));
    }

    std::int64_t end_index_absolute_value = step_size_value > int64_t(0) ? static_cast<std::int64_t>(array_value.size()) - int64_t(1) : int64_t(0);
    if (end_index.has_value())
    {
        const auto end_index_result = try_get<std::int64_t>(end_index.value().evaluate(environment));
        if (! end_index_result.has_value())
        {
            return zeus::unexpected(end_index_result.error());
        }
        const auto end_index_value = end_index_result.value();

        if (end_index_value < int64_t(0))
        {
            end_index_absolute_value = int64_t(array_value.size()) - -end_index_value - int64_t(1);
            end_index_absolute_value = std::max(end_index_absolute_value, int64_t(0));
        }
        else
        {
            end_index_absolute_value = end_index_value;
            end_index_absolute_value = std::min(end_index_absolute_value, static_cast<std::int64_t>(array_value.size()) - int64_t(1));
        }
    }

    std::vector<eval::Value> result;

    if (step_size_value > 0)
    {
        for (std::int64_t i = start_index_absolute_value; i <= end_index_absolute_value; i += step_size_value)
        {
            result.push_back(array_value[size_t(i)]);
        }
    }
    else
    {
        for (std::int64_t i = start_index_absolute_value; i >= end_index_absolute_value; i += step_size_value)
        {
            result.push_back(array_value[size_t(i)]);
        }
    }

    return result;
}

[[nodiscard]] std::unordered_set<std::string> SliceExpr::freeVariables() const noexcept
{
    auto free_vars = array.freeVariables();
    if (start_index.has_value())
    {
        const auto start_index_vars = start_index.value().freeVariables();
        free_vars.insert(start_index_vars.begin(), start_index_vars.end());
    }
    if (end_index.has_value())
    {
        const auto end_index_vars = end_index.value().freeVariables();
        free_vars.insert(end_index_vars.begin(), end_index_vars.end());
    }
    if (step_size.has_value())
    {
        const auto step_size_vars = step_size.value().freeVariables();
        free_vars.insert(step_size_vars.begin(), step_size_vars.end());
    }
    return free_vars;
}

[[nodiscard]] bool SliceExpr::deepEq(const Expr& other) const noexcept
{
    if (const auto other_slice_expr = dynamic_cast<const SliceExpr*>(&other))
    {
        return array.deepEq(other_slice_expr->array) && start_index.has_value() == other_slice_expr->start_index.has_value()
               && (! start_index.has_value() || start_index.value().deepEq(other_slice_expr->start_index.value()))
               && end_index.has_value() == other_slice_expr->end_index.has_value() && (! end_index.has_value() || end_index.value().deepEq(other_slice_expr->end_index.value()))
               && step_size.has_value() == other_slice_expr->step_size.has_value() && (! step_size.has_value() || step_size.value().deepEq(other_slice_expr->step_size.value()));
    }
    return false;
}

void SliceExpr::visitAll(std::function<void(const Expr&)> visitor) const noexcept
{
    visitor(*this);

    array.visitAll(visitor);
    if (start_index.has_value())
    {
        start_index.value().visitAll(visitor);
    }
    if (end_index.has_value())
    {
        end_index.value().visitAll(visitor);
    }
    if (step_size.has_value())
    {
        step_size.value().visitAll(visitor);
    }
}

} // namespace CuraFormulaeEngine::ast
