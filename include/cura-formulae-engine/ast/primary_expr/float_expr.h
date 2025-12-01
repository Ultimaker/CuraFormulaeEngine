#pragma once

#include "primary_expr.h"

namespace CuraFormulaeEngine::ast
{

struct FloatExpr final : PrimaryExpr<double>
{
    using PrimaryExpr::PrimaryExpr;
};

} // namespace CuraFormulaeEngine::ast
