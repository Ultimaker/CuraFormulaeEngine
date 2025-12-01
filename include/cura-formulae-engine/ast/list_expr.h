#pragma once

#include "cura-formulae-engine/ast/ast.h"
#include "expr_ptr.h"
#include "cura-formulae-engine/eval.h"

#include <zeus/expected.hpp>

namespace CuraFormulaeEngine::ast
{

struct ListExpr final : Expr
{
    std::vector<ExprPtr> elements;

    ListExpr(std::vector<ExprPtr> elements)
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
ExprPtr make_list_expr(Ts&&... elements)
{
    std::vector<ExprPtr> elements_vec;
    (elements_vec.push_back(std::forward<Ts>(elements)), ...);
    return make_expr_ptr<ListExpr>(std::move(elements_vec));
}

} // namespace CuraFormulaeEngine::ast
