#include "cura-formulae-engine/env/round.h"

#include <zeus/expected.hpp>

#include <cmath>
#include <limits>
#include <variant>
#include <vector>

namespace CuraFormulaeEngine::env
{

const eval::Value::fn_t round = [](const std::vector<eval::Value> &args) -> eval::Result
{
    if (args.size() > 2)
    {
        return zeus::unexpected(eval::Error::InvalidNumberOfArguments);
    }

    auto base = 0LL;
    if (args.size() == 2)
    {
        const auto& base_arg = args[1];
        if (! std::holds_alternative<std::int64_t>(base_arg.value))
        {
            return zeus::unexpected(eval::Error::TypeMismatch);
        }
        base = std::get<std::int64_t>(base_arg.value);
    }

    const auto& x = args[0];
    const auto initialize = [](const auto& value) -> double
    {
        if (std::holds_alternative<std::int64_t>(value.value))
        {
            return static_cast<double>(std::get<std::int64_t>(value.value));
        }
        if (std::holds_alternative<double>(value.value))
        {
            return std::get<double>(value.value);
        }
        return std::numeric_limits<double>::quiet_NaN();
    };
    double value = initialize(x);
    if (std::isnan(value))
    {
        return zeus::unexpected(eval::Error::TypeMismatch);
    }

    const auto base_exponent = std::pow(10, base);
    value = std::round(value * base_exponent) / base_exponent;
    if (base <= 0)
    {
        return static_cast<std::int64_t>(value);
    }
    return value;
};

} // namespace CuraFormulaeEngine::env
