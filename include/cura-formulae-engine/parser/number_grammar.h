#pragma once

#include "cura-formulae-engine/ast/expr_ptr.h"
#include "cura-formulae-engine/ast/primary_expr/float_expr.h"
#include "cura-formulae-engine/ast/primary_expr/int_expr.h"

#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>
#include <memory>
#include <string>

namespace CuraFormulaeEngine::parser
{

struct NumberGrammar : lexy::token_production
{
    static constexpr auto rule = lexy::dsl::identifier(lexy::dsl::ascii::digit / lexy::dsl::lit_c<'.'>);
    static constexpr auto value = lexy::callback<ast::ExprPtr>(
        [](const auto number)
        {
            const auto number_str = std::string(number.begin(), number.end());
            if (number_str.find('.') != std::string::npos)
            {
                return static_cast<ast::ExprPtr>(std::make_unique<ast::FloatExpr>(std::stod(number_str)));
            }
            return static_cast<ast::ExprPtr>(std::make_unique<ast::IntExpr>(std::stod(number_str)));
        });
};

} // namespace CuraFormulaeEngine::parser
