#pragma once

#include "primary_expr.h"

namespace CuraFormulaeEngine::ast
{

struct StringExpr final : PrimaryExpr<std::string>
{
    using PrimaryExpr::PrimaryExpr;

    [[nodiscard]] std::string toString() const noexcept final;
};

} // namespace CuraFormulaeEngine::ast
