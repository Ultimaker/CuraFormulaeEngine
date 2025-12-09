#pragma once

#include "cura-formulae-engine/ast/expr_ptr.h"
#include "cura-formulae-engine/ast/primary_expr/none_expr.h"

#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>

namespace CuraFormulaeEngine::parser
{

struct NoneGrammar
{
    static constexpr auto rule = LEXY_LIT("None");
    static constexpr auto value = lexy::callback<ast::ExprPtr>([]() { return ast::ExprPtr(std::make_unique<ast::NoneExpr>()); });
};

} // namespace CuraFormulaeEngine::parser
