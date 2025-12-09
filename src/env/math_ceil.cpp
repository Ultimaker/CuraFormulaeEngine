#include "cura-formulae-engine/env/math_ceil.h"

#include <zeus/expected.hpp>

#include <cmath>
#include <variant>

namespace CuraFormulaeEngine::env
{

const eval::Value::fn_t math_ceil = [](const std::vector<eval::Value> &args) -> eval::Result
{
    if (args.size() != 1)
    {
        return zeus::unexpected(eval::Error::InvalidNumberOfArguments);
    }

    const auto& value = args[0];

    if (std::holds_alternative<std::int64_t>(value.value))
    {
        return eval::Value(std::ceil(std::get<std::int64_t>(value.value)));
    }
    if (std::holds_alternative<double>(value.value))
    {
        return eval::Value(std::ceil(std::get<double>(value.value)));
    }
    if (std::holds_alternative<bool>(value.value))
    {
        return eval::Value(std::ceil(std::get<bool>(value.value)));
    }
    return zeus::unexpected(eval::Error::TypeMismatch);
};

} // namespace CuraFormulaeEngine::env
