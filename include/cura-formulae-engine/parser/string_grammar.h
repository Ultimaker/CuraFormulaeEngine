#pragma once

#include "cura-formulae-engine/ast/expr_ptr.h"
#include "cura-formulae-engine/ast/primary_expr/string_expr.h"

#include <lexy/callback/adapter.hpp>
#include <lexy/callback/string.hpp>
#include <lexy/dsl.hpp>
#include <lexy/dsl/identifier.hpp>
#include <lexy/dsl/option.hpp>
#include <lexy/grammar.hpp>
#include <string>

namespace CuraFormulaeEngine::parser
{

struct StringGrammar : lexy::token_production
{
    static constexpr auto InnerStrGrammar
        = lexy::dsl::ascii::alpha_digit_underscore
        / lexy::dsl::lit_c<' '>
        / lexy::dsl::lit_c<','>
        / lexy::dsl::lit_c<'/'>
        / lexy::dsl::lit_c<'.'>
        / lexy::dsl::lit_c<'_'>
        / lexy::dsl::lit_c<'-'>
        / lexy::dsl::lit_c<'+'>
        / lexy::dsl::lit_c<'('>
        / lexy::dsl::lit_c<')'>
        / lexy::dsl::lit_c<';'>
        / lexy::dsl::lit_c<'\\'>
        / lexy::dsl::lit_c<'\n'>
        / lexy::dsl::lit_c<'='>
        / lexy::dsl::lit_c<'{'>
        / lexy::dsl::lit_c<'}'>
        / lexy::dsl::lit_c<'>'>
        / lexy::dsl::lit_c<'<'>
        / lexy::dsl::lit_c<':'>
        / lexy::dsl::lit_c<'/'>
        / lexy::dsl::lit_c<'!'>;
    // clang-format on

    struct DoubleQuoteStringGrammar : token_production
    {
        static constexpr auto rule = lexy::dsl::lit_c<'\"'> >> lexy::dsl::opt(lexy::dsl::identifier(InnerStrGrammar / lexy::dsl::lit_c<'\''>)) >> lexy::dsl::lit_c<'\"'>;
        static constexpr auto value = lexy::as_string<std::string>;
    };

    struct SingleQuoteStringGrammar : token_production
    {
        static constexpr auto rule = lexy::dsl::lit_c<'\''> >> lexy::dsl::opt(lexy::dsl::identifier(InnerStrGrammar / lexy::dsl::lit_c<'\"'>)) >> lexy::dsl::lit_c<'\''>;
        static constexpr auto value = lexy::as_string<std::string>;
    };

    static constexpr auto rule = DoubleQuoteStringGrammar::rule | SingleQuoteStringGrammar::rule;
    static constexpr auto value = lexy::callback<ast::ExprPtr>(
        [](lexy::nullopt = {}) { return CuraFormulaeEngine::ast::make_expr_ptr<ast::StringExpr>(std::string("")); },
        [](const auto& str) { return CuraFormulaeEngine::ast::make_expr_ptr<ast::StringExpr>(std::string(str.begin(), str.end())); });
};

} // namespace CuraFormulaeEngine::parser
