#include "cura-formulae-engine/ast/binary_expr/sub_expr.h"
#include "cura-formulae-engine/eval.h"

#include <string>

namespace CuraFormulaeEngine::ast
{

[[nodiscard]] std::string SubExpr::getOpIdentifier() const noexcept
{
    return "-";
}

eval::Result SubExpr::evaluate(eval::Value& lhs, eval::Value& rhs) const noexcept
{
    return lhs - rhs;
}

} // namespace CuraFormulaeEngine::ast

CuraFormulaeEngine::ast::ExprPtr operator-(CuraFormulaeEngine::ast::ExprPtr lhs, CuraFormulaeEngine::ast::ExprPtr rhs)
{
    return CuraFormulaeEngine::ast::make_expr_ptr<CuraFormulaeEngine::ast::SubExpr>(std::move(lhs), std::move(rhs));
}
