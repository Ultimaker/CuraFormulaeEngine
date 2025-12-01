#pragma once

#include "primary_expr.h"

namespace CuraFormulaeEngine::ast
{

struct NoneExpr final : PrimaryExpr<std::nullptr_t>
{
    NoneExpr()
        : PrimaryExpr(nullptr)
    {
    }

    using PrimaryExpr::PrimaryExpr;
};

} // namespace CuraFormulaeEngine::ast
