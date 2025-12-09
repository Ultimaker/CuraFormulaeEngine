#pragma once

#include "cura-formulae-engine/ast/binary_expr/add_expr.h"
#include "cura-formulae-engine/ast/binary_expr/and_expr.h"
#include "cura-formulae-engine/ast/binary_expr/div_expr.h"
#include "cura-formulae-engine/ast/binary_expr/mod_expr.h"
#include "cura-formulae-engine/ast/binary_expr/mul_expr.h"
#include "cura-formulae-engine/ast/binary_expr/or_expr.h"
#include "cura-formulae-engine/ast/binary_expr/pow_expr.h"
#include "cura-formulae-engine/ast/binary_expr/sub_expr.h"
#include "cura-formulae-engine/ast/comp_chain_expr.h"
#include "cura-formulae-engine/ast/expr_ptr.h"
#include "cura-formulae-engine/ast/unary_expr/neg_expr.h"
#include "cura-formulae-engine/ast/unary_expr/not_expr.h"
#include "bool_grammar.h"
#include "list_grammar.h"
#include "none_grammar.h"
#include "number_grammar.h"
#include "parens_grammar.h"
#include "string_grammar.h"
#include "variable_or_fn_application_grammar_or_array_indexing_grammar.h"

#include <lexy/callback/adapter.hpp>
#include <lexy/callback/fold.hpp>
#include <lexy/callback/forward.hpp>
#include <memory>
#include <utility>

namespace CuraFormulaeEngine::parser
{

struct MathExprGrammar : lexy::expression_production
{
    static constexpr auto whitespace = lexy::dsl::ascii::space;

    // clang-format off
    static constexpr auto atom
        = lexy::dsl::p<BoolGrammar>
        | lexy::dsl::p<NoneGrammar>
        | lexy::dsl::p<NumberGrammar>
        | lexy::dsl::p<ParensGrammar>
        | lexy::dsl::p<StringGrammar>
        | lexy::dsl::p<ListGrammar>
        | lexy::dsl::p<VariableOrFnApplicationGrammarOrArrayIndexingGrammar>;
    // clang-format on

    static constexpr auto op_pow = lexy::dsl::op(LEXY_LIT("**"));
    struct number_power : lexy::dsl::infix_op_right
    {
        static constexpr auto op = op_pow;
        using operand = lexy::dsl::atom;
    };

    static constexpr auto op_neg = lexy::dsl::op(LEXY_LIT("-"));
    struct number_prefix : lexy::dsl::prefix_op
    {
        static constexpr auto op = op_neg;
        using operand = number_power;
    };

    static constexpr auto op_mul = lexy::dsl::op(LEXY_LIT("*"));
    static constexpr auto op_div = lexy::dsl::op(LEXY_LIT("/"));
    static constexpr auto op_mod = lexy::dsl::op(LEXY_LIT("%"));
    struct number_product : lexy::dsl::infix_op_left
    {
        using operand = number_prefix;
        static constexpr auto op = op_mul / op_div / op_mod;
    };


    static constexpr auto op_add = lexy::dsl::op(LEXY_LIT("+"));
    static constexpr auto op_sub = lexy::dsl::op(LEXY_LIT("-"));
    struct number_sum : lexy::dsl::infix_op_left
    {
        using operand = number_product;
        static constexpr auto op = op_add / op_sub;
    };

    static constexpr auto op_eq = lexy::dsl::op(LEXY_LIT("=="));
    static constexpr auto op_neq = lexy::dsl::op(LEXY_LIT("!="));
    static constexpr auto op_lt = lexy::dsl::op(LEXY_LIT("<"));
    static constexpr auto op_gt = lexy::dsl::op(LEXY_LIT(">"));
    static constexpr auto op_leq = lexy::dsl::op(LEXY_LIT("<="));
    static constexpr auto op_geq = lexy::dsl::op(LEXY_LIT(">="));
    static constexpr auto op_not_member = lexy::dsl::op(LEXY_LIT("not in"));
    static constexpr auto op_member = lexy::dsl::op(LEXY_LIT("in"));

    struct number_compare : lexy::dsl::infix_op_list
    {
        using operand = number_sum;
        static constexpr auto op = op_eq / op_neq / op_lt / op_gt / op_leq / op_geq / op_not_member / op_member;
    };

    static constexpr auto op_not = lexy::dsl::op(LEXY_LIT("not"));
    struct bool_prefix : lexy::dsl::prefix_op
    {
        static constexpr auto op = op_not;
        using operand = number_compare;
    };

    static constexpr auto op_and = lexy::dsl::op(LEXY_LIT("and"));
    struct bool_and : lexy::dsl::infix_op_left
    {
        static constexpr auto op = op_and;
        using operand = bool_prefix;
    };

    static constexpr auto op_or = lexy::dsl::op(LEXY_LIT("or"));
    struct bool_or : lexy::dsl::infix_op_left
    {
        static constexpr auto op = op_or;
        using operand = bool_and;
    };

    using operation = bool_or;

    // clang-format off
    static constexpr auto value = lexy::fold_inplace<std::unique_ptr<CuraFormulaeEngine::ast::ComparisonChainExpr>>(
        [] { return std::make_unique<CuraFormulaeEngine::ast::ComparisonChainExpr>(); },
        [](auto&& node, lexy::op<op_eq>) { node->operators.emplace_back(CuraFormulaeEngine::ast::ComparisonOperators::Equals); },
        [](auto&& node, lexy::op<op_neq>) { node->operators.emplace_back(CuraFormulaeEngine::ast::ComparisonOperators::NotEquals); },
        [](auto&& node, lexy::op<op_lt>) { node->operators.emplace_back(CuraFormulaeEngine::ast::ComparisonOperators::LessThan); },
        [](auto&& node, lexy::op<op_gt>) { node->operators.emplace_back(CuraFormulaeEngine::ast::ComparisonOperators::GreaterThan); },
        [](auto&& node, lexy::op<op_leq>) { node->operators.emplace_back(CuraFormulaeEngine::ast::ComparisonOperators::LessThenEqual); },
        [](auto&& node, lexy::op<op_geq>) { node->operators.emplace_back(CuraFormulaeEngine::ast::ComparisonOperators::GreaterThenEqual); },
        [](auto&& node, lexy::op<op_not_member>) { node->operators.emplace_back(CuraFormulaeEngine::ast::ComparisonOperators::NotMember); },
        [](auto&& node, lexy::op<op_member>) { node->operators.emplace_back(CuraFormulaeEngine::ast::ComparisonOperators::Member); },
        [](auto&& node, ast::ExprPtr&& expr) { node->expressions.emplace_back(std::move(expr)); }
    ) >> lexy::callback<ast::ExprPtr>(
        lexy::forward<ast::ExprPtr>,
        [](lexy::op<op_neg>, ast::ExprPtr&& operand) { return - std::move(operand); },
        [](lexy::op<op_not>, ast::ExprPtr&& operand) { return ! std::move(operand); },
        [](ast::ExprPtr&& lhs, lexy::op<op_pow>, ast::ExprPtr&& rhs) { return ast::ExprPtr(std::make_unique<ast::PowExpr>(std::move(lhs), std::move(rhs))); },
        [](ast::ExprPtr&& lhs, lexy::op<op_mul>, ast::ExprPtr&& rhs) { return std::move(lhs) * std::move(rhs); },
        [](ast::ExprPtr&& lhs, lexy::op<op_div>, ast::ExprPtr&& rhs) { return std::move(lhs) / std::move(rhs); },
        [](ast::ExprPtr&& lhs, lexy::op<op_mod>, ast::ExprPtr&& rhs) { return std::move(lhs) % std::move(rhs); },
        [](ast::ExprPtr&& lhs, lexy::op<op_add>, ast::ExprPtr&& rhs) { return std::move(lhs) + std::move(rhs); },
        [](ast::ExprPtr&& lhs, lexy::op<op_sub>, ast::ExprPtr&& rhs) { return std::move(lhs) - std::move(rhs); },
        [](ast::ExprPtr&& lhs, lexy::op<op_and>, ast::ExprPtr&& rhs) { return std::move(lhs) && std::move(rhs); },
        [](ast::ExprPtr&& lhs, lexy::op<op_or>, ast::ExprPtr&& rhs) { return std::move(lhs) || std::move(rhs); }
    );
    // clang-format on
};

} // namespace CuraFormulaeEngine::parser
