#pragma once

#include "cura-formulae-engine/ast/ast.h"
#include "expr_ptr.h"

#include <fmt/format.h>
#include <zeus/expected.hpp>

#include <string>

namespace CuraFormulaeEngine::ast
{

struct TupleExpr final : Expr
{
    std::vector<ExprPtr> elements;

    TupleExpr(std::vector<ExprPtr> elements)
        : elements(std::move(elements))
    {
    }

    [[nodiscard]] std::string toString() const noexcept final;

    [[nodiscard]] eval::Result evaluate(const env::Environment* environment) const noexcept final;

    [[nodiscard]] std::unordered_set<std::string> freeVariables() const noexcept final;

    [[nodiscard]] bool deepEq(const Expr& other) const noexcept final;

    void visitAll(std::function<void(const Expr&)> visitor) const noexcept final;
};

template<typename... Ts>
ExprPtr make_tuple_expr(Ts&&... elements)
{
    std::vector<ExprPtr> elements_vec;
    (elements_vec.push_back(std::forward<Ts>(elements)), ...);
    return make_expr_ptr<TupleExpr>(std::move(elements_vec));
}

} // namespace CuraFormulaeEngine::ast
