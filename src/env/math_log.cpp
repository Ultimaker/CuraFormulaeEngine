#include "cura-formulae-engine/env/math_log.h"

#include <zeus/expected.hpp>

#include <cmath>
#include <limits>
#include <numbers>
#include <variant>

namespace CuraFormulaeEngine::env
{

const eval::Value::fn_t math_log = [](const std::vector<eval::Value> &args) -> eval::Result
{
    if (args.empty() || args.size() > 2)
    {
        return zeus::unexpected(eval::Error::InvalidNumberOfArguments);
    }

    auto initialize = [](const eval::Value& value) -> double
    {
        if (std::holds_alternative<double>(value.value))
        {
            return std::get<double>(value.value);
        }
        if (std::holds_alternative<std::int64_t>(value.value))
        {
            return static_cast<double>(std::get<std::int64_t>(value.value));
        }
        if (std::holds_alternative<bool>(value.value))
        {
            return std::get<bool>(value.value) ? 1.0 : 0.0;
        }
        return std::numeric_limits<double>::quiet_NaN();
    };

    const double x = initialize(args[0]);
    if (std::isnan(x))
    {
        return zeus::unexpected(eval::Error::TypeMismatch);
    }

    double base = std::numbers::e;
    if (args.size() == 2)
    {
        if (std::holds_alternative<double>(args[1].value))
        {
            base = std::get<double>(args[1].value);
        }
        else if (std::holds_alternative<std::int64_t>(args[1].value))
        {
            base = static_cast<double>(std::get<std::int64_t>(args[1].value));
        }
        else if (std::holds_alternative<bool>(args[1].value))
        {
            base = std::get<bool>(args[1].value) ? 1.0 : 0.0;
        }
    }

    return eval::Value(std::log(x) / std::log(base));
};

} // namespace CuraFormulaeEngine::env
