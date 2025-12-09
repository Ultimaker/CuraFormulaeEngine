#pragma once

#include "cura-formulae-engine/ast/expr_ptr.h"
#include "expr_grammar.h"

#include <zeus/expected.hpp>

#include <lexy/action/parse.hpp>
#include <lexy/action/validate.hpp>
#include <lexy/callback/forward.hpp>
#include <lexy/dsl/eof.hpp>
#include <lexy/dsl/production.hpp>
#include <lexy/encoding.hpp>
#include <lexy/grammar.hpp>
#include <lexy/input/string_input.hpp>
#include <string_view>

namespace CuraFormulaeEngine::parser
{

struct Grammar : public lexy::token_production
{
    static constexpr auto rule = lexy::dsl::p<ExprGrammar> + lexy::dsl::eof;
    static constexpr auto value = lexy::forward<ast::ExprPtr>;
};

using error_t = typename lexy::validate_result<lexy::_noop>::error_type;

zeus::expected<ast::ExprPtr, error_t> parse(std::string_view input);

} // namespace CuraFormulaeEngine::parser
