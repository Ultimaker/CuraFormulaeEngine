#include "cura-formulae-engine/ast/property_access_expr.h"
#include "cura-formulae-engine/ast/variable_expr.h"

#include <fmt/format.h>
#include <zeus/expected.hpp>

#include <algorithm>
#include <string>
#include <unordered_set>

namespace CuraFormulaeEngine::ast
{

[[nodiscard]] std::string PropertyAccessExpr::toString() const noexcept
{
    return fmt::format("{}.{}", object.toString(), property);
}

[[nodiscard]] eval::Result PropertyAccessExpr::evaluate(const env::Environment* environment) const noexcept
{
    const auto object_result = object.evaluate(environment);
    if (! object_result.has_value())
    {
        return zeus::unexpected(object_result.error());
    }

    const auto& object_value = object_result.value();

    // Handle property maps (e.g., math.pi)
    if (std::holds_alternative<std::unordered_map<std::string, eval::Value>>(object_value.value))
    {
        const auto& map = std::get<std::unordered_map<std::string, eval::Value>>(object_value.value);
        const auto it = map.find(property);
        if (it == map.end())
        {
            return zeus::unexpected(eval::Error::UndefinedVariable);
        }
        return it->second;
    }

    // Handle built-in methods on lists/vectors (e.g., list.index)
    if (std::holds_alternative<std::vector<eval::Value>>(object_value.value))
    {
        if (property == "index")
        {
            const auto list = object_value;
            return eval::Value::fn_t([list](const std::vector<eval::Value>& args) -> eval::Result
            {
                if (args.size() != 1)
                {
                    return zeus::unexpected(eval::Error::InvalidNumberOfArguments);
                }

                const auto& search_value = args[0];
                const auto& list_items = std::get<std::vector<eval::Value>>(list.value);

                for (size_t i = 0; i < list_items.size(); ++i)
                {
                    if (list_items[i].deepEq(search_value))
                    {
                        return eval::Value(static_cast<int64_t>(i));
                    }
                }

                return zeus::unexpected(eval::Error::ValueError);
            });
        }
        return zeus::unexpected(eval::Error::UndefinedVariable);
    }

    return zeus::unexpected(eval::Error::TypeMismatch);
}

[[nodiscard]] std::unordered_set<std::string> PropertyAccessExpr::freeVariables() const noexcept
{
    return object.freeVariables();
}

[[nodiscard]] bool PropertyAccessExpr::deepEq(const Expr& other) const noexcept
{
    if (const auto& other_property_access = dynamic_cast<const PropertyAccessExpr*>(&other))
    {
        return object.deepEq(other_property_access->object) && property == other_property_access->property;
    }
    return false;
}

void PropertyAccessExpr::visitAll(std::function<void(const Expr&)> visitor) const noexcept
{
    visitor(*this);
    object.visitAll(visitor);
}

} // namespace CuraFormulaeEngine::ast
