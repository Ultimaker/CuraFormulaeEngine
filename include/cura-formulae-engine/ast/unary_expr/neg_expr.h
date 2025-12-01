#pragma once

#include "cura-formulae-engine/ast/expr_ptr.h"
#include "unary_expr.h"
#include "cura-formulae-engine/eval.h"

#include <string>

namespace CuraFormulaeEngine::ast
{

struct NegExpr final : UnaryExpr
{
    using UnaryExpr::UnaryExpr;

    [[nodiscard]] std::string getOpIdentifier() const noexcept final;

    [[nodiscard]] eval::Result evaluate(const eval::Value& eval_value) const noexcept final;
};

} // namespace CuraFormulaeEngine::ast

CuraFormulaeEngine::ast::ExprPtr operator-(CuraFormulaeEngine::ast::ExprPtr operand);
