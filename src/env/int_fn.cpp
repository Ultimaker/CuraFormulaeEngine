#include "cura-formulae-engine/env/int_fn.h"

#include <zeus/expected.hpp>

#include <cmath>
#include <string>
#include <variant>
#include <vector>

namespace CuraFormulaeEngine::env
{

const eval::Value::fn_t int_fn = [](const std::vector<eval::Value> &args) -> eval::Result
{
    if (args.size() == 2)
    {
        if (std::holds_alternative<std::string>(args[0].value) && std::holds_alternative<std::int64_t>(args[1].value))
        {
            const auto& str = std::get<std::string>(args[0].value);
            const auto& base = std::get<std::int64_t>(args[1].value);
            if (base < 2 || base > 36)
            {
                return zeus::unexpected(eval::Error::ValueError);
            }
            return std::int64_t(std::stoll(str, nullptr, int(base)));
        }
        return zeus::unexpected(eval::Error::TypeMismatch);
    }

    if (args.size() != 1)
    {
        return zeus::unexpected(eval::Error::InvalidNumberOfArguments);
    }

    const auto& x = args[0];
    if (std::holds_alternative<std::int64_t>(x.value))
    {
        return std::get<std::int64_t>(x.value);
    }
    if (std::holds_alternative<double>(x.value))
    {
        return static_cast<std::int64_t>(std::round(std::get<double>(x.value)));
    }
    if (std::holds_alternative<bool>(x.value))
    {
        return std::get<bool>(x.value) ? int64_t(1) : int64_t(0);
    }
    if (std::holds_alternative<std::string>(x.value))
    {
        return static_cast<std::int64_t>(std::stod(std::get<std::string>(x.value)));
    }
    return zeus::unexpected(eval::Error::TypeMismatch);
};

} // namespace CuraFormulaeEngine::env
