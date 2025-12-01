#pragma once

#include "primary_expr.h"

namespace CuraFormulaeEngine::ast
{

struct BoolExpr final : PrimaryExpr<bool>
{
    using PrimaryExpr::PrimaryExpr;
};

} // namespace CuraFormulaeEngine::ast
