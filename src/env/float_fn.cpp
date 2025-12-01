#include "cura-formulae-engine/env/float_fn.h"

#include <zeus/expected.hpp>

#include <string>
#include <variant>
#include <vector>

namespace CuraFormulaeEngine::env
{

const eval::Value::fn_t float_fn = [](const std::vector<eval::Value> &args) -> eval::Result
{
    if (args.size() != 1)
    {
        return zeus::unexpected(eval::Error::InvalidNumberOfArguments);
    }

    const auto& value = args[0];
    if (std::holds_alternative<double>(value.value))
    {
        return std::get<double>(value.value);
    }
    if (std::holds_alternative<std::int64_t>(value.value))
    {
        return { std::get<std::int64_t>(value.value) };
    }
    if (std::holds_alternative<std::string>(value.value))
    {
        return std::stod(std::get<std::string>(value.value));
    }
    if (std::holds_alternative<bool>(value.value))
    {
        return std::get<bool>(value.value) ? 1.0 : 0.0;
    }
    return zeus::unexpected(eval::Error::TypeMismatch);
};

} // namespace CuraFormulaeEngine::env
