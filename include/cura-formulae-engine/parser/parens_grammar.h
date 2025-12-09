#pragma once

#include "cura-formulae-engine/ast/expr_ptr.h"
#include "cura-formulae-engine/ast/tuple_expr.h"
#include "nested_grammar.h"

#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>
#include <memory>
#include <vector>

namespace CuraFormulaeEngine::parser
{

struct ParensGrammar : lexy::token_production
{
    static constexpr auto rule
        = lexy::dsl::round_bracketed.opt_list(lexy::dsl::p<NestedGrammar>, lexy::dsl::ignore_trailing_sep(lexy::dsl::comma >> lexy::dsl::whitespace(lexy::dsl::ascii::space)));
    static constexpr auto value = lexy::as_list<std::vector<ast::ExprPtr>> >> lexy::callback<ast::ExprPtr>(
                                      [](std::vector<ast::ExprPtr> exprs)
                                      {
                                          if (exprs.size() == 1)
                                          {
                                              return std::move(exprs[0]);
                                          }
                                          return ast::ExprPtr(std::make_unique<ast::TupleExpr>(std::move(exprs)));
                                      },
                                      [](lexy::nullopt = {}) { return ast::ExprPtr(std::make_unique<ast::TupleExpr>(std::vector<ast::ExprPtr>{})); });
};

} // namespace CuraFormulaeEngine::parser
