#pragma once

#include "cura-formulae-engine/ast/expr_ptr.h"
#include "cura-formulae-engine/ast/list_comprehension_expr.h"
#include "math_expr_grammar.h"
#include "variable_grammar.h"

#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>
#include <memory>
#include <vector>

namespace CuraFormulaeEngine::parser
{

struct ListComprehensionGrammar : lexy::token_production
{
    static constexpr auto whitespace = lexy::dsl::ascii::space;

    struct condition_chain : token_production
    {
        static constexpr auto rule = lexy::dsl::list(LEXY_LIT("if") >> lexy::dsl::whitespace(lexy::dsl::ascii::space) + lexy::dsl::p<MathExprGrammar>);
        static constexpr auto value = lexy::fold_inplace<std::vector<ast::ExprPtr>>(
            []() { return std::vector<ast::ExprPtr>{}; },
            [](auto&& conditions, ast::ExprPtr&& condition) { conditions.emplace_back(std::move(condition)); });
    };

    struct loop : token_production
    {
        static constexpr auto rule = lexy::dsl::list(
            LEXY_LIT("for") >> lexy::dsl::whitespace(lexy::dsl::ascii::space) + lexy::dsl::p<VariableGrammar> + lexy::dsl::whitespace(lexy::dsl::ascii::space) + LEXY_LIT("in")
                                   + lexy::dsl::whitespace(lexy::dsl::ascii::space) + lexy::dsl::p<MathExprGrammar>
                                   + lexy::dsl::whitespace(lexy::dsl::ascii::space) + lexy::dsl::opt(lexy::dsl::p<condition_chain>));

        static constexpr auto value = lexy::fold_inplace<std::vector<ast::ListComprehensionExpr::loop>>(
            []() { return std::vector<ast::ListComprehensionExpr::loop>{}; },
            [](auto&& loops, ast::ExprPtr&& iterator_key, ast::ExprPtr&& iterable, lexy::nullopt)
            { loops.emplace_back(ast::ListComprehensionExpr::loop(std::move(iterator_key), std::move(iterable), std::vector<ast::ExprPtr>{})); },
            [](auto&& loops, ast::ExprPtr&& iterator_key, ast::ExprPtr&& iterable, std::vector<ast::ExprPtr>&& conditions)
            { loops.emplace_back(ast::ListComprehensionExpr::loop(std::move(iterator_key), std::move(iterable), std::move(conditions))); });
    };

    static constexpr auto rule = lexy::dsl::p<MathExprGrammar> + lexy::dsl::opt(lexy::dsl::p<loop>);
    static constexpr auto value = lexy::callback<ast::ExprPtr>(
        [](auto&& expr, lexy::nullopt) { return std::move(expr); },
        [](auto&& expr, auto&& loops) { return ast::ExprPtr(std::make_unique<ast::ListComprehensionExpr>(std::move(expr), std::move(loops))); });
};

} // namespace CuraFormulaeEngine::parser
