#include "cura-formulae-engine/ast/binary_expr/binary_expr.h"
#include "cura-formulae-engine/ast/ast.h"

#include <fmt/format.h>
#include <zeus/expected.hpp>

#include <string>
#include <unordered_set>

namespace CuraFormulaeEngine::ast
{

[[nodiscard]] std::string BinaryExpr::toString() const noexcept
{
    return fmt::format("({} {} {})", lhs.toString(), getOpIdentifier(), rhs.toString());
}

[[nodiscard]] eval::Result BinaryExpr::evaluate(const env::Environment* environment) const noexcept
{
    const auto lhs_result = lhs.evaluate(environment);
    if (! lhs_result.has_value())
    {
        return zeus::unexpected(lhs_result.error());
    }
    auto lhs_eval = lhs_result.value();

    const auto rhs_result = rhs.evaluate(environment);
    if (! rhs_result.has_value())
    {
        return zeus::unexpected(rhs_result.error());
    }
    auto rhs_eval = rhs_result.value();

    return evaluate(lhs_eval, rhs_eval);
}

[[nodiscard]] std::unordered_set<std::string> BinaryExpr::freeVariables() const noexcept
{
    auto lhs_free_variables = lhs.freeVariables();
    auto rhs_free_variables = rhs.freeVariables();
    lhs_free_variables.insert(rhs_free_variables.begin(), rhs_free_variables.end());
    return lhs_free_variables;
}

[[nodiscard]] bool BinaryExpr::deepEq(const Expr& other) const noexcept
{
    if (typeid(*this) != typeid(other))
    {
        return false;
    }
    const auto& other_binary_expr = static_cast<const BinaryExpr&>(other);
    return lhs.deepEq(other_binary_expr.lhs) && rhs.deepEq(other_binary_expr.rhs);
}

void BinaryExpr::visitAll(std::function<void(const Expr&)> visitor) const noexcept
{
    visitor(*this);

    lhs.visitAll(visitor);
    rhs.visitAll(visitor);
}

} // namespace CuraFormulaeEngine::ast
