#pragma once

#include "cura-formulae-engine/ast/ast.h"
#include "expr_ptr.h"

namespace CuraFormulaeEngine::ast
{

enum ComparisonOperators
{
    Equals,
    NotEquals,
    LessThan,
    GreaterThan,
    LessThenEqual,
    GreaterThenEqual,
    NotMember,
    Member,
};

struct ComparisonChainExpr final : Expr
{

    std::vector<ExprPtr> expressions;
    std::vector<ComparisonOperators> operators;

    ComparisonChainExpr() = default;

    ComparisonChainExpr(std::vector<ExprPtr> &&expressions, std::vector<ComparisonOperators> &&operators)
        : expressions(std::move(expressions))
        , operators(std::move(operators))
    {
    }

    [[nodiscard]] std::string toString() const noexcept final;

    [[nodiscard]] eval::Result evaluate(const env::Environment* environment) const noexcept final;

    [[nodiscard]] std::unordered_set<std::string> freeVariables() const noexcept final;

    [[nodiscard]] bool deepEq(const Expr& other) const noexcept final;

    void visitAll(std::function<void(const Expr&)> visitor) const noexcept final;
};

} // namespace CuraFormulaeEngine::ast

CuraFormulaeEngine::ast::ExprPtr operator==(CuraFormulaeEngine::ast::ExprPtr lhs, CuraFormulaeEngine::ast::ExprPtr rhs);
CuraFormulaeEngine::ast::ExprPtr operator!=(CuraFormulaeEngine::ast::ExprPtr lhs, CuraFormulaeEngine::ast::ExprPtr rhs);
CuraFormulaeEngine::ast::ExprPtr operator>=(CuraFormulaeEngine::ast::ExprPtr lhs, CuraFormulaeEngine::ast::ExprPtr rhs);
CuraFormulaeEngine::ast::ExprPtr operator>(CuraFormulaeEngine::ast::ExprPtr lhs, CuraFormulaeEngine::ast::ExprPtr rhs);
CuraFormulaeEngine::ast::ExprPtr operator<=(CuraFormulaeEngine::ast::ExprPtr lhs, CuraFormulaeEngine::ast::ExprPtr rhs);
CuraFormulaeEngine::ast::ExprPtr operator<(CuraFormulaeEngine::ast::ExprPtr lhs, CuraFormulaeEngine::ast::ExprPtr rhs);
