#include "cura-formulae-engine/ast/fn_application_expr.h"
#include "cura-formulae-engine/ast/property_access_expr.h"
#include "cura-formulae-engine/ast/variable_expr.h"

#include <fmt/format.h>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/c_str.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>
#include <zeus/expected.hpp>

#include <algorithm>
#include <cstddef>
#include <optional>
#include <string>
#include <unordered_set>
#include <variant>
#include <vector>

namespace CuraFormulaeEngine::ast
{
namespace
{
std::optional<std::vector<eval::Value>> bindKeywordArguments(
    const std::vector<std::string>& parameter_names,
    const std::vector<eval::Value>& positional_args,
    const std::vector<std::pair<std::string, eval::Value>>& keyword_args
) noexcept
{
    if (positional_args.size() > parameter_names.size())
    {
        return std::nullopt;
    }

    std::vector<std::optional<eval::Value>> slots(parameter_names.size());
    for (size_t i = 0; i < positional_args.size(); ++i)
    {
        slots[i] = positional_args[i];
    }

    for (const auto& [name, value] : keyword_args)
    {
        auto it = std::find(parameter_names.begin(), parameter_names.end(), name);
        if (it == parameter_names.end())
        {
            return std::nullopt;
        }

        const auto idx = static_cast<size_t>(std::distance(parameter_names.begin(), it));
        if (slots[idx].has_value())
        {
            return std::nullopt;
        }
        slots[idx] = value;
    }

    std::vector<eval::Value> bound;
    for (const auto& slot : slots)
    {
        if (! slot.has_value())
        {
            break;
        }
        bound.push_back(slot.value());
    }

    // If any slot beyond the consecutively-filled slots from the start has a
    // value there is a gap (a required earlier argument is missing while a
    // later one was supplied via keyword).
    // Return nullopt so the caller can report InvalidNumberOfArguments.
    for (size_t i = bound.size(); i < slots.size(); ++i)
    {
        if (slots[i].has_value())
        {
            return std::nullopt;
        }
    }

    return bound;
}

} // namespace

[[nodiscard]] std::string FnApplicationExpr::toString() const noexcept
{
    std::vector<std::string> all_args;
    all_args.reserve(args.size() + kwargs.size());

    for (const auto& arg : args)
    {
        all_args.push_back(arg.toString());
    }
    for (const auto& kwarg : kwargs)
    {
        all_args.push_back(fmt::format("{}={}", kwarg.name, kwarg.value.toString()));
    }

    auto args_str = all_args | ranges::views::join(ranges::views::c_str(", ")) | ranges::to<std::string>();

    if (const auto& variable = dynamic_cast<const VariableExpr*>(fn.ptr.get()))
    {
        return fmt::format("({}({}))", variable->name, args_str);
    }
    return fmt::format("(({})({}))", fn.toString(), args_str);
}

[[nodiscard]] eval::Result FnApplicationExpr::evaluate(const env::Environment* environment) const noexcept
{
    const auto fn_result = fn.evaluate(environment);
    if (! fn_result.has_value())
    {
        return zeus::unexpected(fn_result.error());
    }

    eval::Value::fn_t fn_value;
    std::optional<std::vector<std::string>> parameter_names;
    const auto& fn_variant = fn_result.value().value;
    if (std::holds_alternative<eval::Value::fn_t>(fn_variant))
    {
        fn_value = std::get<eval::Value::fn_t>(fn_variant);
    }
    else if (std::holds_alternative<eval::Value::rich_fn_t>(fn_variant))
    {
        const auto& rich_fn = std::get<eval::Value::rich_fn_t>(fn_variant);
        fn_value = rich_fn.operation;
        parameter_names = rich_fn.getSignature();
    }
    else
    {
        return zeus::unexpected(eval::Error::TypeMismatch);
    }

    std::vector<eval::Value> positional_arg_results;
    positional_arg_results.reserve(args.size());
    for (const auto& arg : args)
    {
        const auto arg_result = arg.evaluate(environment);
        if (! arg_result.has_value())
        {
            return zeus::unexpected(arg_result.error());
        }
        positional_arg_results.push_back(arg_result.value());
    }

    if (kwargs.empty())
    {
        return fn_value(positional_arg_results);
    }

    std::vector<std::pair<std::string, eval::Value>> keyword_arg_results;
    keyword_arg_results.reserve(kwargs.size());
    for (const auto& kwarg : kwargs)
    {
        const auto value_result = kwarg.value.evaluate(environment);
        if (! value_result.has_value())
        {
            return zeus::unexpected(value_result.error());
        }
        keyword_arg_results.emplace_back(kwarg.name, value_result.value());
    }

    if (! parameter_names.has_value())
    {
        return zeus::unexpected(eval::Error::InvalidNumberOfArguments);
    }

    const auto bound_args = bindKeywordArguments(parameter_names.value(), positional_arg_results, keyword_arg_results);
    if (! bound_args.has_value())
    {
        return zeus::unexpected(eval::Error::InvalidNumberOfArguments);
    }

    return fn_value(bound_args.value());
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
    for (const auto& kwarg : kwargs)
    {
        const auto kwarg_vars = kwarg.value.freeVariables();
        result.insert(kwarg_vars.begin(), kwarg_vars.end());
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

        if (kwargs.size() != other_fn_application->kwargs.size())
        {
            return false;
        }
        for (size_t i = 0; i < kwargs.size(); ++i)
        {
            if (! kwargs[i].deepEq(other_fn_application->kwargs[i]))
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
    for (const auto& kwarg : kwargs)
    {
        kwarg.value.visitAll(visitor);
    }
}

} // namespace CuraFormulaeEngine::ast
