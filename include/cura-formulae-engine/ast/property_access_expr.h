#pragma once

#include "cura-formulae-engine/ast/ast.h"
#include "expr_ptr.h"

#include <string>

namespace CuraFormulaeEngine::ast
{

struct PropertyAccessExpr final : Expr
{
    ExprPtr object;
    std::string property;

    PropertyAccessExpr(ExprPtr object, std::string property)
        : object(std::move(object))
        , property(std::move(property))
    {
    }

    [[nodiscard]] std::string toString() const noexcept final;

    [[nodiscard]] eval::Result evaluate(const env::Environment* environment) const noexcept final;

    [[nodiscard]] std::unordered_set<std::string> freeVariables() const noexcept final;

    [[nodiscard]] bool deepEq(const Expr& other) const noexcept final;

    void visitAll(std::function<void(const Expr&)> visitor) const noexcept final;
};

} // namespace CuraFormulaeEngine::ast
