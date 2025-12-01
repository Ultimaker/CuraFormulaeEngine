#include "cura-formulae-engine/ast/unary_expr/not_expr.h"

#include <string>

namespace CuraFormulaeEngine::ast
{

[[nodiscard]] std::string NotExpr::getOpIdentifier() const noexcept
{
    return "not";
}

[[nodiscard]] eval::Result NotExpr::evaluate(const eval::Value& eval_value) const noexcept
{
    return ! eval_value;
}

} // namespace CuraFormulaeEngine::ast

CuraFormulaeEngine::ast::ExprPtr operator!(CuraFormulaeEngine::ast::ExprPtr operand)
{
    return CuraFormulaeEngine::ast::make_expr_ptr<CuraFormulaeEngine::ast::NotExpr>(std::move(operand));
}
