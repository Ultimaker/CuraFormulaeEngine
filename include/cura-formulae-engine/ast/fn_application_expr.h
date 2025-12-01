#pragma once

#include "cura-formulae-engine/ast/ast.h"
#include "expr_ptr.h"

namespace CuraFormulaeEngine::ast
{

struct FnApplicationExpr final : Expr
{
    ExprPtr fn;
    std::vector<ExprPtr> args;

    FnApplicationExpr(ExprPtr fn, std::vector<ExprPtr> args)
        : fn(std::move(fn))
        , args(std::move(args))
    {
    }

    [[nodiscard]] std::string toString() const noexcept final;

    [[nodiscard]] eval::Result evaluate(const env::Environment* environment) const noexcept final;

    [[nodiscard]] std::unordered_set<std::string> freeVariables() const noexcept final;

    [[nodiscard]] bool deepEq(const Expr& other) const noexcept final;

    void visitAll(std::function<void(const Expr&)> visitor) const noexcept final;
};

} // namespace CuraFormulaeEngine::ast

template<typename... Ts>
CuraFormulaeEngine::ast::ExprPtr CuraFormulaeEngine::ast::ExprPtr::operator()(Ts... args)
{
    std::vector<ExprPtr> args_vec;
    (args_vec.push_back(std::forward<Ts>(args)), ...);
    return make_expr_ptr<FnApplicationExpr>(std::move(*this), std::move(args_vec));
}
