#include "cura-formulae-engine/env/sum.h"

#include <zeus/expected.hpp>

#include <variant>
#include <vector>

namespace CuraFormulaeEngine::env
{

const eval::Value::fn_t sum = [](const std::vector<eval::Value> &args) -> eval::Result
{
    if (args.size() != 1)
    {
        return zeus::unexpected(eval::Error::InvalidNumberOfArguments);
    }

    if (! std::holds_alternative<std::vector<eval::Value>>(args[0].value))
    {
        return zeus::unexpected(eval::Error::TypeMismatch);
    }

    auto result = eval::Value{ int64_t(0) };
    const auto list = std::get<std::vector<eval::Value>>(args[0].value);
    for (const auto& arg : list)
    {
        const auto sum = result + arg;
        if (! sum.has_value())
        {
            return zeus::unexpected(sum.error());
        }
        result = sum.value();
    }
    return result;
};

} // namespace CuraFormulaeEngine::env
