#pragma once

#include "cura-formulae-engine/ast/expr_ptr.h"
#include "cura-formulae-engine/ast/fn_application_expr.h"
#include "cura-formulae-engine/ast/index_expr.h"
#include "cura-formulae-engine/ast/slice_expr.h"
#include "nested_grammar.h"
#include "variable_grammar.h"

#include <spdlog/spdlog.h>

#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>
#include <lexy/dsl/literal.hpp>
#include <memory>
#include <optional>

namespace CuraFormulaeEngine::parser
{

struct ApplyExpr
{
    ApplyExpr() = default;
    ApplyExpr(const ApplyExpr&) = default;
    ApplyExpr(ApplyExpr&&) = default;
    ApplyExpr& operator=(const ApplyExpr&) = default;
    ApplyExpr& operator=(ApplyExpr&&) = default;

    virtual ~ApplyExpr() = default;
    virtual ast::ExprPtr apply(ast::ExprPtr&&) = 0;
};

struct IndexExpr final : ApplyExpr
{
    IndexExpr() = default;

    IndexExpr(std::optional<ast::ExprPtr>&& index)
        : index(std::move(index))
    {
    }

    std::optional<ast::ExprPtr> index;

    ast::ExprPtr apply(ast::ExprPtr&& array) override { return std::move(array)[std::move(index.value())]; }
};

struct ApplySliceExpr final : ApplyExpr
{
    ApplySliceExpr() = default;

    ApplySliceExpr(std::optional<ast::ExprPtr>&& start_index, std::optional<ast::ExprPtr>&& end_index, std::optional<ast::ExprPtr>&& step)
        : start_index(std::move(start_index))
        , end_index(std::move(end_index))
        , step(std::move(step))
    {
    }

    std::optional<ast::ExprPtr> start_index;
    std::optional<ast::ExprPtr> end_index;
    std::optional<ast::ExprPtr> step;

    ast::ExprPtr apply(ast::ExprPtr&& array) override
    {
        return { std::make_unique<ast::SliceExpr>(std::move(array), std::move(start_index), std::move(end_index), std::move(step)) };
    }
};

struct ApplyFnApplicationExpr final : ApplyExpr
{
    ApplyFnApplicationExpr() = default;

    ApplyFnApplicationExpr(std::vector<ast::ExprPtr>&& args)
        : args(std::move(args))
    {
    }

    std::vector<ast::ExprPtr> args;

    ast::ExprPtr apply(ast::ExprPtr&& variable) override { return { std::make_unique<ast::FnApplicationExpr>(std::move(variable), std::move(args)) }; }
};

struct VariableOrFnApplicationGrammarOrArrayIndexingGrammar : lexy::token_production
{
    struct SliceElementColon : token_production
    {
        static constexpr auto rule = lexy::dsl::if_(lexy::dsl::peek_not(lexy::dsl::colon) >> lexy::dsl::p<NestedGrammar>);
        static constexpr auto value = lexy::callback<std::optional<ast::ExprPtr>>([](ast::ExprPtr expr) { return expr; }, []() { return std::nullopt; });
    };

    struct SliceElementColonOrBracket : token_production
    {
        static constexpr auto rule = lexy::dsl::if_(lexy::dsl::peek_not(lexy::dsl::colon | lexy::dsl::lit_c<']'>) >> lexy::dsl::p<NestedGrammar>);
        static constexpr auto value = lexy::callback<std::optional<ast::ExprPtr>>([](ast::ExprPtr expr) { return expr; }, []() { return std::nullopt; });
    };

    struct FnApplicationGrammar : token_production
    {
        struct FnApplicationGrammarInner : token_production
        {
            static constexpr auto whitespace = lexy::dsl::whitespace(lexy::dsl::ascii::space);
            static constexpr auto rule = lexy::dsl::round_bracketed.opt_list(lexy::dsl::p<NestedGrammar>, lexy::dsl::ignore_trailing_sep(lexy::dsl::comma));
            static constexpr auto value
                = lexy::as_list<std::vector<ast::ExprPtr>> >> lexy::callback<std::unique_ptr<ApplyExpr>>(
                      [](lexy::nullopt = {}) -> std::unique_ptr<ApplyExpr> { return std::make_unique<ApplyFnApplicationExpr>(); },
                      [](std::vector<ast::ExprPtr> args) -> std::unique_ptr<ApplyExpr> { return std::make_unique<ApplyFnApplicationExpr>(std::move(args)); });
        };

        static constexpr auto rule = lexy::dsl::peek(lexy::dsl::lit_c<'('>) >> lexy::dsl::p<FnApplicationGrammarInner>;
        static constexpr auto value = lexy::forward<std::unique_ptr<ApplyExpr>>;
    };

    struct IndexOrSliceGrammar : token_production
    {
        static constexpr auto rule
            = lexy::dsl::peek(lexy::dsl::lit_c<'['>)
           >> lexy::dsl::square_bracketed(lexy::dsl::p<SliceElementColon> + lexy::dsl::times<2>(lexy::dsl::if_(lexy::dsl::colon >> lexy::dsl::p<SliceElementColonOrBracket>)));
        static constexpr auto value = lexy::callback<std::unique_ptr<ApplyExpr>>(
            [](std::optional<ast::ExprPtr>&& index) { return std::make_unique<IndexExpr>(std::move(index)); },
            [](std::optional<ast::ExprPtr>&& start_index, std::optional<ast::ExprPtr>&& end_index)
            { return std::make_unique<ApplySliceExpr>(std::move(start_index), std::move(end_index), std::nullopt); },
            [](std::optional<ast::ExprPtr>&& start_index, std::optional<ast::ExprPtr>&& end_index, std::optional<ast::ExprPtr>&& step)
            { return std::make_unique<ApplySliceExpr>(std::move(start_index), std::move(end_index), std::move(step)); });
    };

    struct List : token_production
    {
        static constexpr auto rule = lexy::dsl::list(lexy::dsl::p<FnApplicationGrammar> | lexy::dsl::p<IndexOrSliceGrammar>);
        static constexpr auto value = lexy::as_list<std::vector<std::unique_ptr<ApplyExpr>>>;
    };

    static constexpr auto rule = lexy::dsl::p<VariableGrammar> >> lexy::dsl::if_(lexy::dsl::peek(lexy::dsl::lit_c<'['> | lexy::dsl::lit_c<'('>) >> lexy::dsl::p<List>);
    static constexpr auto value = lexy::callback<ast::ExprPtr>(
        [](auto&& expr) { return std::forward<decltype(expr)>(expr); },
        [](auto&& expr, lexy::nullopt) { return std::forward<decltype(expr)>(expr); },
        [](auto&& expr, const std::vector<std::unique_ptr<ApplyExpr>>& apply_exprs)
        {
            for (const std::unique_ptr<ApplyExpr>& apply_expr : apply_exprs)
            {
                expr = apply_expr->apply(std::forward<decltype(expr)>(expr));
            }
            return std::forward<decltype(expr)>(expr);
        });
};

} // namespace CuraFormulaeEngine::parser
