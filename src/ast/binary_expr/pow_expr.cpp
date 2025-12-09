#include "cura-formulae-engine/ast/binary_expr/pow_expr.h"
#include "cura-formulae-engine/eval.h"

#include <string>

namespace CuraFormulaeEngine::ast
{

[[nodiscard]] std::string PowExpr::getOpIdentifier() const noexcept
{
    return "**";
}

eval::Result PowExpr::evaluate(eval::Value& lhs, eval::Value& rhs) const noexcept
{
    return eval::pow(lhs, rhs);
}

} // namespace CuraFormulaeEngine::ast
