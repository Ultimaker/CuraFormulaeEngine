#include "cura-formulae-engine/env/abs.h"

#include <zeus/expected.hpp>

#include <cmath>
#include <variant>
#include <vector>

namespace CuraFormulaeEngine::env {

const eval::Value::fn_t abs = [](const std::vector<eval::Value> &args) -> eval::Result
{
    if (args.size() != 1)
    {
        return zeus::unexpected(eval::Error::InvalidNumberOfArguments);
    }

    const auto &value = args[0];
    if (std::holds_alternative<bool>(value.value))
    {
        return std::get<bool>(value.value) ? eval::Value(int64_t(1)) : eval::Value(int64_t(0));
    }
    if (std::holds_alternative<double>(value.value))
    {
        return std::abs(std::get<double>(value.value));
    }
    if (std::holds_alternative<std::int64_t>(value.value))
    {
        return std::abs(std::get<std::int64_t>(value.value));
    }


    return zeus::unexpected(eval::Error::TypeMismatch);
};

} // namespace CuraFormulaeEngine::env
