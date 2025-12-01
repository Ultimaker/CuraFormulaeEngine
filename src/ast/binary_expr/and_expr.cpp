#include "cura-formulae-engine/ast/binary_expr/and_expr.h"
#include "cura-formulae-engine/ast/expr_ptr.h"
#include "cura-formulae-engine/eval.h"

#include <string>
#include <variant>

namespace CuraFormulaeEngine::ast
{

[[nodiscard]] std::string AndExpr::getOpIdentifier() const noexcept
{
    return "and";
}

eval::Result AndExpr::evaluate(eval::Value& lhs, eval::Value& rhs) const noexcept
{
    return lhs && rhs;
}

} // namespace CuraFormulaeEngine::ast

CuraFormulaeEngine::ast::ExprPtr operator&&(CuraFormulaeEngine::ast::ExprPtr lhs, CuraFormulaeEngine::ast::ExprPtr rhs)
{
    return CuraFormulaeEngine::ast::make_expr_ptr<CuraFormulaeEngine::ast::AndExpr>(std::move(lhs), std::move(rhs));
}
