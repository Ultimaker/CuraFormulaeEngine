#pragma once

#include "binary_expr.h"
#include "cura-formulae-engine/ast/expr_ptr.h"
#include "cura-formulae-engine/eval.h"

#include <string>
#include <variant>

namespace CuraFormulaeEngine::ast
{

struct OrExpr final : BinaryExpr
{
    using BinaryExpr::BinaryExpr;

    [[nodiscard]] std::string getOpIdentifier() const noexcept final;

    eval::Result evaluate(eval::Value& lhs, eval::Value& rhs) const noexcept final;
};

} // namespace CuraFormulaeEngine::ast

CuraFormulaeEngine::ast::ExprPtr operator||(CuraFormulaeEngine::ast::ExprPtr lhs, CuraFormulaeEngine::ast::ExprPtr rhs);
