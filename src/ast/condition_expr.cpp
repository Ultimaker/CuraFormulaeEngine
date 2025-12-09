#include "cura-formulae-engine/ast/condition_expr.h"
#include "cura-formulae-engine/ast/ast.h"

#include <fmt/format.h>
#include <zeus/expected.hpp>

#include <string>
#include <unordered_set>

namespace CuraFormulaeEngine::ast
{

[[nodiscard]] std::string ConditionExpr::toString() const noexcept
{
    return fmt::format("({} if {} else {})", then_expr.toString(), condition.toString(), else_expr.toString());
}

[[nodiscard]] eval::Result ConditionExpr::evaluate(const env::Environment* environment) const noexcept
{
    const auto condition_result = condition.evaluate(environment);
    if (! condition_result.has_value())
    {
        return zeus::unexpected(condition_result.error());
    }
    const auto& condition_value = condition_result.value();

    if (condition_value.isTruthy())
    {
        return then_expr.evaluate(environment);
    }
    return else_expr.evaluate(environment);
}

[[nodiscard]] std::unordered_set<std::string> ConditionExpr::freeVariables() const noexcept
{
    auto condition_vars = condition.freeVariables();
    auto then_vars = then_expr.freeVariables();
    auto else_vars = else_expr.freeVariables();
    condition_vars.insert(then_vars.begin(), then_vars.end());
    condition_vars.insert(else_vars.begin(), else_vars.end());
    return condition_vars;
}

[[nodiscard]] bool ConditionExpr::deepEq(const Expr& other) const noexcept
{
    if (const auto other_condition_expr = dynamic_cast<const ConditionExpr*>(&other))
    {
        // clang-format off
        return then_expr.deepEq(other_condition_expr->then_expr)
               && condition.deepEq(other_condition_expr->condition)
               && else_expr.deepEq(other_condition_expr->else_expr);
        // clang-format on
    }
    return false;
}

void ConditionExpr::visitAll(std::function<void(const Expr&)> visitor) const noexcept
{
    visitor(*this);

    then_expr.visitAll(visitor);
    condition.visitAll(visitor);
    else_expr.visitAll(visitor);
}

} // namespace CuraFormulaeEngine::ast
