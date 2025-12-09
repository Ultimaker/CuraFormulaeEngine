#pragma once

#include "cura-formulae-engine/ast/ast.h"
#include "cura-formulae-engine/ast/expr_ptr.h"
#include "cura-formulae-engine/eval.h"

namespace CuraFormulaeEngine::ast
{

struct BinaryExpr : Expr
{
    ExprPtr lhs;
    ExprPtr rhs;

    BinaryExpr(ExprPtr lhs, ExprPtr rhs)
        : lhs(std::move(lhs))
        , rhs(std::move(rhs))
    {
    }

    [[nodiscard]] virtual std::string getOpIdentifier() const = 0;

    [[nodiscard]] std::string toString() const noexcept final;

    virtual eval::Result evaluate(eval::Value& lhs, eval::Value& rhs) const = 0;

    [[nodiscard]] eval::Result evaluate(const env::Environment* environment) const noexcept final;

    [[nodiscard]] std::unordered_set<std::string> freeVariables() const noexcept final;

    [[nodiscard]] bool deepEq(const Expr& other) const noexcept final;

    void visitAll(std::function<void(const Expr&)> visitor) const noexcept final;
};

} // namespace CuraFormulaeEngine::ast
