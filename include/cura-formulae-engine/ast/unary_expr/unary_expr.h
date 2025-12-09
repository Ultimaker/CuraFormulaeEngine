#pragma once

#include "cura-formulae-engine/ast/ast.h"
#include "cura-formulae-engine/ast/expr_ptr.h"
#include "cura-formulae-engine/eval.h"

#include <fmt/format.h>
#include <zeus/expected.hpp>

#include <functional>
#include <string>
#include <unordered_set>
#include <variant>

namespace CuraFormulaeEngine::ast
{

struct UnaryExpr : Expr
{
    ExprPtr operand;

    explicit UnaryExpr(ExprPtr operand)
        : operand(std::move(operand))
    {
    }

    [[nodiscard]] virtual std::string getOpIdentifier() const = 0;

    [[nodiscard]] std::string toString() const noexcept final;

    [[nodiscard]] virtual eval::Result evaluate(const eval::Value& eval_value) const = 0;

    [[nodiscard]] eval::Result evaluate(const env::Environment* environment) const noexcept final;

    [[nodiscard]] std::unordered_set<std::string> freeVariables() const noexcept final;

    [[nodiscard]] bool deepEq(const Expr& other) const noexcept final;

    void visitAll(std::function<void(const Expr&)> visitor) const noexcept final;
};

} // namespace CuraFormulaeEngine::ast
