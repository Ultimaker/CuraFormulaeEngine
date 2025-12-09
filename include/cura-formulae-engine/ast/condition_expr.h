#pragma once

#include "cura-formulae-engine/ast/ast.h"
#include "expr_ptr.h"

namespace CuraFormulaeEngine::ast
{

struct ConditionExpr final : Expr
{
    ExprPtr then_expr;
    ExprPtr condition;
    ExprPtr else_expr;

    ConditionExpr(ExprPtr then_expr, ExprPtr condition, ExprPtr else_expr)
        : then_expr(std::move(then_expr))
        , condition(std::move(condition))
        , else_expr(std::move(else_expr))
    {
    }

    [[nodiscard]] std::string toString() const noexcept final;

    [[nodiscard]] eval::Result evaluate(const env::Environment* environment) const noexcept final;

    [[nodiscard]] std::unordered_set<std::string> freeVariables() const noexcept final;

    [[nodiscard]] bool deepEq(const Expr& other) const noexcept final;

    void visitAll(std::function<void(const Expr&)> visitor) const noexcept final;
};

} // namespace CuraFormulaeEngine::ast
