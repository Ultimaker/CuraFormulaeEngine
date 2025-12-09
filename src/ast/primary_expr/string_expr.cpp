#include "cura-formulae-engine/ast/primary_expr/string_expr.h"

#include <fmt/format.h>

#include <string>

namespace CuraFormulaeEngine::ast
{

std::string StringExpr::toString() const noexcept
{
    return fmt::format("\"{}\"", value);
}

} // namespace CuraFormulaeEngine::ast
