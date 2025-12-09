#pragma once

#include "primary_expr.h"

#include <cstdint>

namespace CuraFormulaeEngine::ast
{

struct IntExpr final : PrimaryExpr<std::int64_t>
{
    using PrimaryExpr::PrimaryExpr;
};

} // namespace CuraFormulaeEngine::ast
