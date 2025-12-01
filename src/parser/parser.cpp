#include "cura-formulae-engine/ast/expr_ptr.h"
#include "cura-formulae-engine/parser/parser.h"

#include <zeus/expected.hpp>

#include <lexy/action/parse.hpp>
#include <lexy/action/validate.hpp>
#include <lexy/encoding.hpp>
#include <lexy/input/string_input.hpp>
#include <string_view>

namespace CuraFormulaeEngine::parser
{

zeus::expected<ast::ExprPtr, error_t> parse(std::string_view input)
{
    auto parse_result = lexy::parse<Grammar>(lexy::string_input<lexy::utf8_encoding>(input), lexy::noop);
    if (parse_result.is_success())
    {
        return static_cast<lexy::parse_result<ast::ExprPtr, lexy::_noop>&&>(parse_result).value();
    }
    return zeus::unexpected(parse_result.errors());
}

} // namespace CuraFormulaeEngine::parser
