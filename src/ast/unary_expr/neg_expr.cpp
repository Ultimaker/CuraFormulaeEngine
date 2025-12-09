#include "cura-formulae-engine/ast/unary_expr/neg_expr.h"

#include <string>

namespace CuraFormulaeEngine::ast
{

[[nodiscard]] std::string NegExpr::getOpIdentifier() const noexcept
{
    return "-";
}

[[nodiscard]] eval::Result NegExpr::evaluate(const eval::Value& eval_value) const noexcept
{
    return -eval_value;
}

} // namespace CuraFormulaeEngine::ast

CuraFormulaeEngine::ast::ExprPtr operator-(CuraFormulaeEngine::ast::ExprPtr operand)
{
    return CuraFormulaeEngine::ast::make_expr_ptr<CuraFormulaeEngine::ast::NegExpr>(std::move(operand));
}
