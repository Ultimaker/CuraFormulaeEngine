#include "cura-formulae-engine/ast/unary_expr/unary_expr.h"

#include <fmt/format.h>
#include <zeus/expected.hpp>

#include <string>
#include <unordered_set>

namespace CuraFormulaeEngine::ast
{

[[nodiscard]] std::string UnaryExpr::toString() const noexcept
{
    return fmt::format("({} {})", getOpIdentifier(), operand.toString());
}

[[nodiscard]] eval::Result UnaryExpr::evaluate(const env::Environment* environment) const noexcept
{
    const auto operand_result = operand.evaluate(environment);
    if (! operand_result.has_value())
    {
        return zeus::unexpected(operand_result.error());
    }
    const auto operand_value = operand_result.value();
    return evaluate(operand_value);
}

[[nodiscard]] std::unordered_set<std::string> UnaryExpr::freeVariables() const noexcept
{
    return operand.freeVariables();
}

[[nodiscard]] bool UnaryExpr::deepEq(const Expr& other) const noexcept
{
    if (typeid(*this) != typeid(other))
    {
        return false;
    }
    const auto& other_UnaryExpr = static_cast<const UnaryExpr&>(other);
    return operand.deepEq(other_UnaryExpr.operand);
}

void UnaryExpr::visitAll(std::function<void(const Expr&)> visitor) const noexcept
{
    visitor(*this);
    operand.visitAll(visitor);
}

} // namespace CuraFormulaeEngine::ast
