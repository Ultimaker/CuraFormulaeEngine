#include "cura-formulae-engine/env/map.h"

#include <zeus/expected.hpp>

#include <variant>
#include <vector>

namespace CuraFormulaeEngine::env
{

const eval::Value::fn_t map = [](const std::vector<eval::Value> &args) -> eval::Result
{
    if (args.size() != 2)
    {
        return zeus::unexpected(eval::Error::InvalidNumberOfArguments);
    }

    if (! std::holds_alternative<eval::Value::fn_t>(args[0].value))
    {
        return zeus::unexpected(eval::Error::TypeMismatch);
    }
    if (! std::holds_alternative<std::vector<eval::Value>>(args[1].value))
    {
        return zeus::unexpected(eval::Error::TypeMismatch);
    }

    const auto fn = std::get<eval::Value::fn_t>(args[0].value);
    const auto list = std::get<std::vector<eval::Value>>(args[1].value);

    std::vector<eval::Value> result;
    for (const auto& element : list)
    {
        const auto mapped = fn({ element });
        if (! mapped.has_value())
        {
            return zeus::unexpected(eval::Error::TypeMismatch);
        }
        result.push_back(mapped.value());
    }
    return result;
};

} // namespace CuraFormulaeEngine::env
