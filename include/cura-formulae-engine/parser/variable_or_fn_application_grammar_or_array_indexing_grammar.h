#pragma once

#include "cura-formulae-engine/ast/expr_ptr.h"
#include "cura-formulae-engine/ast/fn_application_expr.h"
#include "cura-formulae-engine/ast/index_expr.h"
#include "cura-formulae-engine/ast/property_access_expr.h"
#include "cura-formulae-engine/ast/slice_expr.h"
#include "bool_grammar.h"
#include "list_grammar.h"
#include "nested_grammar.h"
#include "none_grammar.h"
#include "number_grammar.h"
#include "parens_grammar.h"
#include "string_grammar.h"
#include "variable_grammar.h"

#include <spdlog/spdlog.h>

#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>
#include <lexy/dsl/literal.hpp>
#include <memory>
#include <optional>
#include <string>
#include <variant>

namespace CuraFormulaeEngine::parser {

struct ApplyExpr {
  ApplyExpr() = default;
  ApplyExpr(const ApplyExpr &) = default;
  ApplyExpr(ApplyExpr &&) = default;
  ApplyExpr &operator=(const ApplyExpr &) = default;
  ApplyExpr &operator=(ApplyExpr &&) = default;

  virtual ~ApplyExpr() = default;
  virtual ast::ExprPtr apply(ast::ExprPtr &&) = 0;
};

struct IndexExpr final : ApplyExpr {
  IndexExpr() = default;

  IndexExpr(std::optional<ast::ExprPtr> &&index) : index(std::move(index)) {}

  std::optional<ast::ExprPtr> index;

  ast::ExprPtr apply(ast::ExprPtr &&array) override {
    return std::move(array)[std::move(index.value())];
  }
};

struct ApplySliceExpr final : ApplyExpr {
  ApplySliceExpr() = default;

  ApplySliceExpr(std::optional<ast::ExprPtr> &&start_index,
                 std::optional<ast::ExprPtr> &&end_index,
                 std::optional<ast::ExprPtr> &&step)
      : start_index(std::move(start_index)), end_index(std::move(end_index)),
        step(std::move(step)) {}

  std::optional<ast::ExprPtr> start_index;
  std::optional<ast::ExprPtr> end_index;
  std::optional<ast::ExprPtr> step;

  ast::ExprPtr apply(ast::ExprPtr &&array) override {
    return {std::make_unique<ast::SliceExpr>(
        std::move(array), std::move(start_index), std::move(end_index),
        std::move(step))};
  }
};

struct ApplyFnApplicationExpr final : ApplyExpr
{
    using FnArgElement = std::variant<ast::ExprPtr, ast::FnApplicationExpr::KeywordArg>;

    ApplyFnApplicationExpr() = default;

  ApplyFnApplicationExpr(std::vector<ast::ExprPtr> &&args)
      : args(std::move(args)) {}

    ApplyFnApplicationExpr(std::vector<FnArgElement>&& arg_elements)
    {
        for (auto& arg : arg_elements)
        {
            if (std::holds_alternative<ast::ExprPtr>(arg))
            {
                args.push_back(std::move(std::get<ast::ExprPtr>(arg)));
            }
            else
            {
                kwargs.push_back(std::move(std::get<ast::FnApplicationExpr::KeywordArg>(arg)));
            }
        }
    }

    std::vector<ast::ExprPtr> args;
    std::vector<ast::FnApplicationExpr::KeywordArg> kwargs;

    ast::ExprPtr apply(ast::ExprPtr&& variable) override
    {
        return { std::make_unique<ast::FnApplicationExpr>(std::move(variable), std::move(args), std::move(kwargs)) };
    }
};

struct ApplyPropertyAccessExpr final : ApplyExpr
{
    ApplyPropertyAccessExpr() = default;

    explicit ApplyPropertyAccessExpr(std::string property_name)
        : property_name(std::move(property_name))
    {
    }

    std::string property_name;

    ast::ExprPtr apply(ast::ExprPtr&& object) override
    {
        return { std::make_unique<ast::PropertyAccessExpr>(std::move(object), std::move(property_name)) };
    }
};

struct VariableOrFnApplicationGrammarOrArrayIndexingGrammar
    : lexy::token_production {
  struct SliceElementColon : token_production {
    static constexpr auto rule = lexy::dsl::if_(
        lexy::dsl::peek_not(lexy::dsl::colon) >> lexy::dsl::p<NestedGrammar>);
    static constexpr auto value = lexy::callback<std::optional<ast::ExprPtr>>(
        [](ast::ExprPtr expr) { return expr; }, []() { return std::nullopt; });
  };

  struct SliceElementColonOrBracket : token_production {
    static constexpr auto rule = lexy::dsl::if_(
        lexy::dsl::peek_not(lexy::dsl::colon | lexy::dsl::lit_c<']'>) >>
        lexy::dsl::p<NestedGrammar>);
    static constexpr auto value = lexy::callback<std::optional<ast::ExprPtr>>(
        [](ast::ExprPtr expr) { return expr; }, []() { return std::nullopt; });
  };

    struct FnApplicationGrammar : token_production
    {
        struct FnKeywordArgGrammar : token_production
        {
            static constexpr auto rule
                = lexy::dsl::peek(lexy::dsl::identifier(lexy::dsl::ascii::alpha_digit_underscore) >> lexy::dsl::lit_c<'='> >> lexy::dsl::peek_not(lexy::dsl::lit_c<'='>))
               >> lexy::dsl::identifier(lexy::dsl::ascii::alpha_digit_underscore) + lexy::dsl::lit_c<'='> + lexy::dsl::p<NestedGrammar>;

            static constexpr auto value = lexy::callback<ast::FnApplicationExpr::KeywordArg>(
                [](const auto& identifier, ast::ExprPtr value)
                {
                    ast::FnApplicationExpr::KeywordArg kwarg;
                    kwarg.name = std::string(identifier.begin(), identifier.end());
                    kwarg.value = std::move(value);
                    return kwarg;
                });
        };

        struct FnArgElementGrammar : token_production
        {
            static constexpr auto rule = lexy::dsl::p<FnKeywordArgGrammar> | (lexy::dsl::else_ >> lexy::dsl::p<NestedGrammar>);
            static constexpr auto value = lexy::callback<ApplyFnApplicationExpr::FnArgElement>(
                [](ast::FnApplicationExpr::KeywordArg kwarg) -> ApplyFnApplicationExpr::FnArgElement { return kwarg; },
                [](ast::ExprPtr arg) -> ApplyFnApplicationExpr::FnArgElement { return arg; });
        };

        struct FnApplicationGrammarInner : token_production
        {
            static constexpr auto whitespace = lexy::dsl::whitespace(lexy::dsl::ascii::space);
            static constexpr auto rule = lexy::dsl::round_bracketed.opt_list(lexy::dsl::p<FnArgElementGrammar>, lexy::dsl::ignore_trailing_sep(lexy::dsl::comma));
            static constexpr auto value
                = lexy::as_list<std::vector<ApplyFnApplicationExpr::FnArgElement>> >> lexy::callback<std::unique_ptr<ApplyExpr>>(
                      [](lexy::nullopt = {}) -> std::unique_ptr<ApplyExpr> { return std::make_unique<ApplyFnApplicationExpr>(); },
                      [](std::vector<ApplyFnApplicationExpr::FnArgElement> args) -> std::unique_ptr<ApplyExpr>
                      { return std::make_unique<ApplyFnApplicationExpr>(std::move(args)); });
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

    struct PropertyAccessGrammar : token_production
    {
        static constexpr auto rule = lexy::dsl::peek(lexy::dsl::lit_c<'.'>)
                                   >> lexy::dsl::lit_c<'.'> + lexy::dsl::identifier(lexy::dsl::ascii::alpha_digit_underscore);
        static constexpr auto value = lexy::callback<std::unique_ptr<ApplyExpr>>(
            [](const auto& property_name)
            {
                return std::make_unique<ApplyPropertyAccessExpr>(std::string(property_name.begin(), property_name.end()));
            });
    };

    struct List : token_production
    {
        static constexpr auto rule = lexy::dsl::list(lexy::dsl::p<FnApplicationGrammar> | lexy::dsl::p<PropertyAccessGrammar> | lexy::dsl::p<IndexOrSliceGrammar>);
        static constexpr auto value = lexy::as_list<std::vector<std::unique_ptr<ApplyExpr>>>;
    };

    struct PrimaryExpr : token_production
    {
        static constexpr auto rule = lexy::dsl::p<BoolGrammar>
                                   | lexy::dsl::p<NoneGrammar>
                                   | lexy::dsl::p<NumberGrammar>
                                   | lexy::dsl::p<ParensGrammar>
                                   | lexy::dsl::p<StringGrammar>
                                   | lexy::dsl::p<ListGrammar>;
        static constexpr auto value = lexy::forward<ast::ExprPtr>;
    };

    static constexpr auto rule = (lexy::dsl::p<PrimaryExpr> >> lexy::dsl::if_(lexy::dsl::peek(lexy::dsl::lit_c<'['> | lexy::dsl::lit_c<'('> | lexy::dsl::lit_c<'.'>) >> lexy::dsl::p<List>))
                              | (lexy::dsl::p<VariableGrammar> >> lexy::dsl::if_(lexy::dsl::peek(lexy::dsl::lit_c<'['> | lexy::dsl::lit_c<'('> | lexy::dsl::lit_c<'.'>) >> lexy::dsl::p<List>));
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
