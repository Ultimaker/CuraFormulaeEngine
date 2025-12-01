#pragma once

#include "cura-formulae-engine/ast/condition_expr.h"
#include "cura-formulae-engine/ast/expr_ptr.h"
#include "list_comprehension_grammar.h"
#include "nested_grammar.h"

#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>

namespace CuraFormulaeEngine::parser
{

struct ExprGrammar : lexy::expression_production
{
    static constexpr auto whitespace = lexy::dsl::ascii::space;

    static constexpr auto atom = lexy::dsl::p<ListComprehensionGrammar>;

    static constexpr auto op_condition = lexy::dsl::op(LEXY_LIT("if") >> lexy::dsl::p<NestedGrammar> + LEXY_LIT("else"));
    struct Condition : lexy::dsl::infix_op_right
    {
        static constexpr auto op = op_condition;
        using operand = lexy::dsl::atom;
    };

    using operation = Condition;

    static constexpr auto value = lexy::callback<ast::ExprPtr>(
        lexy::forward<ast::ExprPtr>,
        [](ast::ExprPtr&& lhs, lexy::op<op_condition>, ast::ExprPtr&& condition, ast::ExprPtr&& rhs)
        { return ast::ExprPtr(std::make_unique<ast::ConditionExpr>(std::move(lhs), std::move(condition), std::move(rhs))); });
};

} // namespace CuraFormulaeEngine::parser
