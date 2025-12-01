#pragma once

#include "cura-formulae-engine/ast/expr_ptr.h"
#include "cura-formulae-engine/ast/variable_expr.h"

#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>

namespace CuraFormulaeEngine::parser
{

struct VariableGrammar : lexy::token_production
{
    static constexpr auto rule = lexy::dsl::identifier(lexy::dsl::ascii::alpha_digit_underscore / lexy::dsl::lit_c<'.'>);
    static constexpr auto value = lexy::callback<ast::ExprPtr>([](const auto&& variable)
    {
        return ast::ExprPtr(std::make_unique<ast::VariableExpr>(std::string(variable.begin(), variable.end())));
    });
};

} // namespace CuraFormulaeEngine::parser
