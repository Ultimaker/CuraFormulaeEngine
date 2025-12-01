#include "cura-formulae-engine/env/math_degrees.h"

#include <zeus/expected.hpp>

#include <numbers>

namespace CuraFormulaeEngine::env
{

const eval::Value::fn_t math_degrees = [](const std::vector<eval::Value> &args) -> eval::Result
{
    if (args.size() != 1)
    {
        return zeus::unexpected(eval::Error::InvalidNumberOfArguments);
    }
    return args[0] * eval::Value(180.0 / std::numbers::pi);
};

} // namespace CuraFormulaeEngine::env
