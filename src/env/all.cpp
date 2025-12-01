#include "cura-formulae-engine/env/all.h"

#include <range/v3/algorithm/all_of.hpp>
#include <zeus/expected.hpp>

#include <variant>
#include <vector>

namespace CuraFormulaeEngine::env
{

const eval::Value::fn_t all = [](const std::vector<eval::Value> &args) -> eval::Result
{
    if (args.size() != 1)
    {
        return zeus::unexpected(eval::Error::InvalidNumberOfArguments);
    }
    if (! std::holds_alternative<std::vector<eval::Value>>(args[0].value))
    {
        return zeus::unexpected(eval::Error::TypeMismatch);
    }
    const auto& elements = std::get<std::vector<eval::Value>>(args[0].value);
    return ranges::all_of(elements, [](const auto& element) { return element.isTruthy(); });
};

} // namespace CuraFormulaeEngine::env
