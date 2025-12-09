#pragma once

#include "cura-formulae-engine/ast/expr_ptr.h"

#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>

namespace CuraFormulaeEngine::parser
{

// for recursive grammars, equivalent to `struct expr_grammar` but
// written with a forward declaration
struct NestedGrammar : lexy::transparent_production
{
    static constexpr auto rule = lexy::dsl::recurse<struct ExprGrammar>;
    static constexpr auto value = lexy::forward<ast::ExprPtr>;
};

} // namespace CuraFormulaeEngine::parser
