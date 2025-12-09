#pragma once

#include "cura-formulae-engine/ast/ast.h"

namespace CuraFormulaeEngine::ast
{

struct ExprPtr final : Expr
{
    ExprPtr() = default;
    ExprPtr(const ExprPtr&) = delete;
    ExprPtr(ExprPtr&&) = default;
    ExprPtr& operator=(const ExprPtr&) = delete;
    ExprPtr& operator=(ExprPtr&&) = default;
    ~ExprPtr() override = default;

    std::unique_ptr<Expr> ptr;

    ExprPtr(std::unique_ptr<Expr> ptr)
        : ptr(std::move(ptr))
    {
    }

    [[nodiscard]] eval::Result evaluate(const env::Environment* environment) const noexcept final;

    [[nodiscard]] std::unordered_set<std::string> freeVariables() const noexcept final;

    [[nodiscard]] std::string toString() const noexcept final;

    [[nodiscard]] bool deepEq(const Expr& other) const noexcept final;

    void visitAll(std::function<void(const Expr&)> visitor) const noexcept final;

    ExprPtr operator[](ExprPtr other);

    template<typename... Ts>
    ExprPtr operator()(Ts... args);
};

template<typename T, typename... Ts>
ExprPtr make_expr_ptr(Ts... args)
{
    return ExprPtr(std::move(std::make_unique<T>(std::forward<Ts>(args)...)));
}

} // namespace CuraFormulaeEngine::ast
