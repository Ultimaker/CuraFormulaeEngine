#pragma once

#include "cura-formulae-engine/ast/expr_ptr.h"
#include "cura-formulae-engine/ast/primary_expr/bool_expr.h"

#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>
#include <memory>

namespace CuraFormulaeEngine::parser
{

struct BoolGrammar : lexy::token_production
{
    struct TrueGrammar
    {
        static constexpr auto rule = LEXY_LIT("True");
        static constexpr auto value = lexy::callback<ast::ExprPtr>([]() { return ast::ExprPtr(std::make_unique<ast::BoolExpr>(true)); });
    };

    struct FalseGrammar
    {
        static constexpr auto rule = LEXY_LIT("False");
        static constexpr auto value = lexy::callback<ast::ExprPtr>([]() { return ast::ExprPtr(std::make_unique<ast::BoolExpr>(false)); });
    };

    static constexpr auto rule = lexy::dsl::p<TrueGrammar> | lexy::dsl::p<FalseGrammar>;
    static constexpr auto value = lexy::forward<ast::ExprPtr>;
};

} // namespace CuraFormulaeEngine::parser
