#include "cura-formulae-engine/ast/list_comprehension_expr.h"
#include "cura-formulae-engine/ast/ast.h"
#include "cura-formulae-engine/eval.h"

#include <fmt/format.h>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/c_str.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>

#include <optional>
#include <unordered_set>
#include <variant>
#include <vector>
#include <zeus/expected.hpp>

namespace CuraFormulaeEngine::ast
{

[[nodiscard]] std::string ListComprehensionExpr::toString() const noexcept
{
    const auto loops_str = loops
                           | ranges::views::transform(
            [](const auto& loop)
            {
                if (loop.conditions.empty())
                {
                    return fmt::format("for {} in {}", loop.iterator_key.toString(), loop.iterable.toString());
                }
                const auto conditions_str = loop.conditions
                                            | ranges::views::transform([](const auto& condition) { return fmt::format("if {}", condition.toString()); })
                                            | ranges::views::join(ranges::views::c_str(" ")) | ranges::to<std::string>();
                return fmt::format("for {} in {} {}", loop.iterator_key.toString(), loop.iterable.toString(), conditions_str);
            })
                           | ranges::views::join(ranges::views::c_str(" ")) | ranges::to<std::string>();

    return fmt::format("({} {})", iterator.toString(), loops_str);
}

std::optional<eval::Error> ListComprehensionExpr::handle_loop(const size_t loop_index, env::LocalEnvironment& local_environment, std::vector<eval::Value>& results) const
{
    if (loop_index >= loops.size())
    {
        assert(loops.size() == 0 && "loops in list comprehension cannot be empty");
        throw std::runtime_error("loops in list comprehension cannot be empty");
    }

    const auto& loop = loops[loop_index];
    const auto iterable_result = try_get<std::vector<eval::Value>>(loop.iterable.evaluate(&local_environment));
    if (! iterable_result.has_value())
    {
        return iterable_result.error();
    }
    const auto iterable_value = iterable_result.value();

    for (const auto& element : iterable_value)
    {
        for (const auto& key : loop.iterator_key.freeVariables())
        {
            local_environment.set(key, element);
        }

        auto exit_loop = false;
        for (const auto& condition : loop.conditions)
        {
            const auto condition_result = condition.evaluate(&local_environment);
            if (! condition_result.has_value())
            {
                return condition_result.error();
            }
            const auto& condition_value = condition_result.value();

            if (! condition_value.isTruthy())
            {
                exit_loop = true;
                break;
            }
        }

        if (exit_loop)
        {
            continue;
        }

        if (loop_index + 1 == loops.size())
        {
            const auto iterator_result = iterator.evaluate(&local_environment);
            if (! iterator_result.has_value())
            {
                return iterator_result.error();
            }
            results.push_back(iterator_result.value());
        }
        else
        {
            const auto loop_err = handle_loop(loop_index + 1, local_environment, results);
            if (loop_err.has_value())
            {
                return loop_err.value();
            }
        }
    }

    return std::nullopt;
}

[[nodiscard]] eval::Result ListComprehensionExpr::evaluate(const env::Environment* environment) const noexcept
{
    env::LocalEnvironment local_environment { environment };
    std::vector<eval::Value> results;
    const auto loop_err = handle_loop(0, local_environment, results);
    if (loop_err.has_value())
    {
        return zeus::unexpected(loop_err.value());
    }
    return eval::Value{ results };
}

[[nodiscard]] std::unordered_set<std::string> ListComprehensionExpr::freeVariables() const noexcept
{
    std::unordered_set<std::string> free_variables = {};
    std::unordered_set<std::string> local_variables = {};
    for (const auto& loop : loops)
    {
        for (const auto& key : loop.iterable.freeVariables())
        {
            if (local_variables.contains(key))
            {
                continue;
            }
            free_variables.insert(key);
        }
        for (const auto& condition : loop.conditions)
        {
            for (const auto& key : condition.freeVariables())
            {
                if (local_variables.contains(key))
                {
                    continue;
                }
                free_variables.insert(key);
            }
        }
        for (const auto& key : loop.iterator_key.freeVariables())
        {
            local_variables.insert(key);
        }
    }
    return free_variables;
}

[[nodiscard]] bool ListComprehensionExpr::deepEq(const Expr& other) const noexcept
{
    if (const auto other_list_comprehension = dynamic_cast<const ListComprehensionExpr*>(&other))
    {
        if (loops.size() != other_list_comprehension->loops.size())
        {
            return false;
        }

        for (const auto [loop, other_loop] : ranges::views::zip(loops, other_list_comprehension->loops))
        {
            if (! loop.iterator_key.deepEq(other_loop.iterator_key))
            {
                return false;
            }
            if (! loop.iterable.deepEq(other_loop.iterable))
            {
                return false;
            }
            if (loop.conditions.size() != other_loop.conditions.size())
            {
                return false;
            }
            for (const auto [condition, other_condition] : ranges::views::zip(loop.conditions, other_loop.conditions))
            {
                if (! condition.deepEq(other_condition))
                {
                    return false;
                }
            }
        }

        return iterator.deepEq(other_list_comprehension->iterator);
    }
    return false;
}

void ListComprehensionExpr::visitAll(std::function<void(const Expr&)> visitor) const noexcept
{
    visitor(*this);

    iterator.visitAll(visitor);

    for (const auto& loop : loops)
    {
        loop.iterator_key.visitAll(visitor);
        loop.iterable.visitAll(visitor);
        for (const auto& condition : loop.conditions)
        {
            condition.visitAll(visitor);
        }
    }
}

} // namespace CuraFormulaeEngine::ast
