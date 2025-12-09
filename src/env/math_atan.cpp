#include "cura-formulae-engine/env/math_atan.h"

#include <zeus/expected.hpp>

#include <cmath>
#include <variant>

namespace CuraFormulaeEngine::env
{

const eval::Value::fn_t math_atan = [](const std::vector<eval::Value> &args) -> eval::Result
{
    if (args.size() != 1)
    {
        return zeus::unexpected(eval::Error::InvalidNumberOfArguments);
    }

    const auto& value = args[0];
    if (std::holds_alternative<double>(value.value))
    {
        return std::atan(std::get<double>(value.value));
    }
    if (std::holds_alternative<std::int64_t>(value.value))
    {
        return std::atan(std::get<std::int64_t>(value.value));
    }
    return zeus::unexpected(eval::Error::TypeMismatch);
};

} // namespace CuraFormulaeEngine::env
