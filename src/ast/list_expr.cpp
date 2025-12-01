#include "cura-formulae-engine/ast/list_expr.h"
#include "cura-formulae-engine/ast/ast.h"
#include "cura-formulae-engine/ast/expr_ptr.h"
#include "cura-formulae-engine/eval.h"

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/c_str.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/zip.hpp>
#include <zeus/expected.hpp>

#include <string>
#include <unordered_set>
#include <variant>
#include <vector>

namespace CuraFormulaeEngine::ast
{


[[nodiscard]] std::string ListExpr::toString() const noexcept
{
    auto elements_str = elements | ranges::views::transform([](const auto& element) { return element.toString(); }) | ranges::views::join(ranges::views::c_str(", "))
                        | ranges::to<std::string>();

    return fmt::format("[{}]", elements_str);
}

[[nodiscard]] eval::Result ListExpr::evaluate(const env::Environment* environment) const noexcept
{
    std::vector<eval::Value> results;
    for (const auto& element : elements)
    {
        const auto result = element.evaluate(environment);
        if (! result.has_value())
        {
            return zeus::unexpected(result.error());
        }
        results.push_back(result.value());
    }
    return std::move(results);
}

[[nodiscard]] std::unordered_set<std::string> ListExpr::freeVariables() const noexcept
{
    std::unordered_set<std::string> result;
    for (const auto& element : elements)
    {
        const auto element_vars = element.freeVariables();
        result.insert(element_vars.begin(), element_vars.end());
    }
    return result;
}

[[nodiscard]] bool ListExpr::deepEq(const Expr& other) const noexcept
{
    if (auto other_list_expr = dynamic_cast<const ListExpr*>(&other))
    {
        if (elements.size() != other_list_expr->elements.size())
        {
            return false;
        }

        for (const auto [element, other_element] : ranges::views::zip(elements, other_list_expr->elements))
        {
            if (! element.deepEq(other_element))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

void ListExpr::visitAll(std::function<void(const Expr&)> visitor) const noexcept
{
    visitor(*this);

    for (const auto& element : elements)
    {
        element.visitAll(visitor);
    }
}

} // namespace CuraFormulaeEngine::ast
