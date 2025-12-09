#pragma once

#include "cura-formulae-engine/ast/ast.h"
#include "expr_ptr.h"
#include "cura-formulae-engine/eval.h"

#include <fmt/format.h>
#include <zeus/expected.hpp>

#include <string>
#include <unordered_set>
#include <variant>

namespace CuraFormulaeEngine::ast
{

struct IndexExpr final : Expr
{
    ExprPtr array;
    ExprPtr index;

    IndexExpr(ExprPtr array, ExprPtr index)
        : array(std::move(array))
        , index(std::move(index))
    {
    }

    [[nodiscard]] std::string toString() const noexcept final;

    [[nodiscard]] eval::Result evaluate(const env::Environment* environment) const noexcept final;

    [[nodiscard]] std::unordered_set<std::string> freeVariables() const noexcept final;

    [[nodiscard]] bool deepEq(const Expr& other) const noexcept final;

    void visitAll(std::function<void(const Expr&)> visitor) const noexcept final;
};

} // namespace CuraFormulaeEngine::ast
