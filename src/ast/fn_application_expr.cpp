#include "cura-formulae-engine/ast/fn_application_expr.h"
#include "cura-formulae-engine/ast/variable_expr.h"

#include <fmt/format.h>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/c_str.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>
#include <zeus/expected.hpp>

#include <string>
#include <unordered_set>
#include <variant>
#include <vector>

namespace CuraFormulaeEngine::ast
{

[[nodiscard]] std::string FnApplicationExpr::toString() const noexcept
{
    auto args_str
            = args | ranges::views::transform([](const auto& arg) { return arg.toString(); }) | ranges::views::join(ranges::views::c_str(", ")) | ranges::to<std::string>();

    if (const auto& variable = dynamic_cast<const VariableExpr*>(fn.ptr.get()))
    {
        return fmt::format("({}({}))", variable->name, args_str);
    }
    return fmt::format("(({})({}))", fn.toString(), args_str);
}

[[nodiscard]] eval::Result FnApplicationExpr::evaluate(const env::Environment* environment) const noexcept
{
    const auto fn_result = try_get<eval::Value::fn_t>(fn.evaluate(environment));
    if (! fn_result.has_value())
    {
        return zeus::unexpected(fn_result.error());
    }
    const auto& fn_value = fn_result.value();

    std::vector<eval::Value> arg_results;
    for (const auto& arg : args)
    {
        const auto arg_result = arg.evaluate(environment);
        if (! arg_result.has_value())
        {
            return zeus::unexpected(arg_result.error());
        }
        arg_results.push_back(arg_result.value());
    }

    return fn_value(arg_results);
}

[[nodiscard]] std::unordered_set<std::string> FnApplicationExpr::freeVariables() const noexcept
{
    std::unordered_set<std::string> result;
    const auto fn_vars = fn.freeVariables();
    result.insert(fn_vars.begin(), fn_vars.end());
    for (const auto& arg : args)
    {
        const auto arg_vars = arg.freeVariables();
        result.insert(arg_vars.begin(), arg_vars.end());
    }
    return result;
}

[[nodiscard]] bool FnApplicationExpr::deepEq(const Expr& other) const noexcept
{
    if (const auto& other_fn_application = dynamic_cast<const FnApplicationExpr*>(&other))
    {
        if (! fn.deepEq(other_fn_application->fn))
        {
            return false;
        }
        if (args.size() != other_fn_application->args.size())
        {
            return false;
        }
        for (size_t i = 0; i < args.size(); ++i)
        {
            if (! args[i].deepEq(other_fn_application->args[i]))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

void FnApplicationExpr::visitAll(std::function<void(const Expr&)> visitor) const noexcept
{
    visitor(*this);

    fn.visitAll(visitor);
    for (const auto& arg : args)
    {
        arg.visitAll(visitor);
    }
}

} // namespace CuraFormulaeEngine::ast
