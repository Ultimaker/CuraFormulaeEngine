#include "cura-formulae-engine/ast/binary_expr/add_expr.h"
#include "cura-formulae-engine/ast/expr_ptr.h"
#include "cura-formulae-engine/eval.h"

#include <string>

namespace CuraFormulaeEngine::ast
{

[[nodiscard]] std::string AddExpr::getOpIdentifier() const noexcept
{
    return "+";
}

eval::Result AddExpr::evaluate(eval::Value& lhs, eval::Value& rhs) const noexcept
{
    return lhs + rhs;
}

} // namespace CuraFormulaeEngine::ast

CuraFormulaeEngine::ast::ExprPtr operator+(CuraFormulaeEngine::ast::ExprPtr lhs, CuraFormulaeEngine::ast::ExprPtr rhs)
{
    return CuraFormulaeEngine::ast::make_expr_ptr<CuraFormulaeEngine::ast::AddExpr>(std::move(lhs), std::move(rhs));
}
