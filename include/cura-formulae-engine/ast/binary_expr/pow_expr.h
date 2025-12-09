#pragma once

#include "binary_expr.h"
#include "cura-formulae-engine/eval.h"

#include <string>

namespace CuraFormulaeEngine::ast
{

struct PowExpr final : BinaryExpr
{
    using BinaryExpr::BinaryExpr;

    [[nodiscard]] std::string getOpIdentifier() const noexcept final;

    eval::Result evaluate(eval::Value& lhs, eval::Value& rhs) const noexcept final;
};

} // namespace CuraFormulaeEngine::ast
