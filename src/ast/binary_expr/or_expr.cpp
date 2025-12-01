#include "cura-formulae-engine/ast/binary_expr/or_expr.h"
#include "cura-formulae-engine/eval.h"

#include <string>
#include <variant>

namespace CuraFormulaeEngine::ast
{

[[nodiscard]] std::string OrExpr::getOpIdentifier() const noexcept
{
    return "or";
}

eval::Result OrExpr::evaluate(eval::Value& lhs, eval::Value& rhs) const noexcept
{
    return lhs || rhs;
}

} // namespace CuraFormulaeEngine::ast

CuraFormulaeEngine::ast::ExprPtr operator||(CuraFormulaeEngine::ast::ExprPtr lhs, CuraFormulaeEngine::ast::ExprPtr rhs)
{
    return CuraFormulaeEngine::ast::make_expr_ptr<CuraFormulaeEngine::ast::OrExpr>(std::move(lhs), std::move(rhs));
}
