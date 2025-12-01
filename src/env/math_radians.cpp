#include "cura-formulae-engine/env/math_radians.h"

#include <zeus/expected.hpp>

#include <numbers>

namespace CuraFormulaeEngine::env
{

const eval::Value::fn_t math_radians = [](const std::vector<eval::Value> &args) -> eval::Result
{
    if (args.size() != 1)
    {
        return zeus::unexpected(eval::Error::InvalidNumberOfArguments);
    }
    return args[0] * eval::Value(std::numbers::pi / 180.0);
};

} // namespace CuraFormulaeEngine::env
