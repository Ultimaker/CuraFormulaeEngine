#pragma once

#include "cura-formulae-engine/ast/ast.h"
#include "expr_ptr.h"
#include "cura-formulae-engine/eval.h"

#include <zeus/expected.hpp>

#include <algorithm>
#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace CuraFormulaeEngine::ast
{

struct SliceExpr final : Expr
{
    ExprPtr array;
    std::optional<ExprPtr> start_index;
    std::optional<ExprPtr> end_index;
    std::optional<ExprPtr> step_size;

    SliceExpr(ExprPtr array, std::optional<ExprPtr> start_index, std::optional<ExprPtr> end_index, std::optional<ExprPtr> step_size)
        : array(std::move(array))
        , start_index(std::move(start_index))
        , end_index(std::move(end_index))
        , step_size(std::move(step_size))
    {
    }

    [[nodiscard]] std::string toString() const noexcept final;

    [[nodiscard]] eval::Result evaluate(const env::Environment* environment) const noexcept final;

    [[nodiscard]] std::unordered_set<std::string> freeVariables() const noexcept final;

    [[nodiscard]] bool deepEq(const Expr& other) const noexcept final;

    void visitAll(std::function<void(const Expr&)> visitor) const noexcept final;

};

} // namespace CuraFormulaeEngine::ast
