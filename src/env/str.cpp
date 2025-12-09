#include "cura-formulae-engine/env/str.h"

#include <stdexcept>
#include <string>
#include <variant>
#include <vector>
#include <zeus/expected.hpp>

namespace CuraFormulaeEngine::env
{

const eval::Value::fn_t str = [](const std::vector<eval::Value> &args) -> eval::Result
{
    if (args.size() != 1)
    {
        return zeus::unexpected(eval::Error::InvalidNumberOfArguments);
    }

    if (std::holds_alternative<std::string>(args[0].value))
    {
        return std::get<std::string>(args[0].value);
    }

    if (std::holds_alternative<std::int64_t>(args[0].value))
    {
        return std::to_string(std::get<std::int64_t>(args[0].value));
    }

    if (std::holds_alternative<double>(args[0].value))
    {
        return std::to_string(std::get<double>(args[0].value));
    }

    if (std::holds_alternative<bool>(args[0].value))
    {
        return std::get<bool>(args[0].value) ? std::string("True") : std::string("False");
    }
    throw std::runtime_error("Unimplemented");
};

} // namespace CuraFormulaeEngine::env
