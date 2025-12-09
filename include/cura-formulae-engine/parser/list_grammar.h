#pragma once

#include "cura-formulae-engine/ast/list_comprehension_expr.h"
#include "cura-formulae-engine/ast/list_expr.h"
#include "nested_grammar.h"

#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>
#include <vector>

namespace CuraFormulaeEngine::parser
{

struct ListGrammar : lexy::token_production
{
    struct ListGrammarInner : token_production
    {
        static constexpr auto whitespace = lexy::dsl::ascii::space;
        static constexpr auto rule = lexy::dsl::square_bracketed.opt_list(lexy::dsl::p<NestedGrammar>, lexy::dsl::ignore_trailing_sep(lexy::dsl::comma));
        static constexpr auto value = lexy::as_list<std::vector<ast::ExprPtr>> >> lexy::callback<ast::ExprPtr>(
                                          [](std::vector<ast::ExprPtr> exprs)
                                          {
                                              // special case if the list contains a single list comprehension, we evaluate it and return the result
                                              if (exprs.size() == 1 && dynamic_cast<const ast::ListComprehensionExpr*>(exprs[0].ptr.get()) != nullptr)
                                              {
                                                  return std::move(exprs[0]);
                                              }
                                              return ast::ExprPtr(std::make_unique<ast::ListExpr>(std::move(exprs)));
                                          },
                                          [](lexy::nullopt = {}) { return ast::ExprPtr(std::make_unique<ast::ListExpr>(std::vector<ast::ExprPtr>{})); });
    };

    static constexpr auto rule = lexy::dsl::peek(lexy::dsl::lit_c<'['>) >> lexy::dsl::p<ListGrammarInner>;
    static constexpr auto value = lexy::forward<ast::ExprPtr>;
};

} // namespace CuraFormulaeEngine::parser
