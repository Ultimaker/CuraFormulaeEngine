#include "cura-formulae-engine/env/len.h"

#include <zeus/expected.hpp>

#include <variant>
#include <vector>

namespace CuraFormulaeEngine::env
{

const eval::Value::fn_t len = [](const std::vector<eval::Value> &args) -> eval::Result
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
    return eval::Value{ static_cast<std::int64_t>(elements.size()) };
};

} // namespace CuraFormulaeEngine::env
