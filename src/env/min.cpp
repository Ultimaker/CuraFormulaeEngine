#include "cura-formulae-engine/env/min.h"

#include <range/v3/view/drop.hpp>
#include <zeus/expected.hpp>

#include <variant>
#include <vector>

namespace CuraFormulaeEngine::env
{

const eval::Value::fn_t min = [](const std::vector<eval::Value> &args) -> eval::Result
{
    if (args.empty())
    {
        return zeus::unexpected(eval::Error::InvalidNumberOfArguments);
    }

    const auto find_min = [](const std::vector<eval::Value>& vec) -> eval::Result
    {
        auto min = vec[0];
        for (const auto& arg : vec | ranges::views::drop(1))
        {
            const auto cmp = arg < min;
            if (! cmp.has_value())
            {
                return zeus::unexpected(eval::Error::TypeMismatch);
            }

            if (cmp.value())
            {
                min = arg;
            }
        }
        return min;
    };

    if (args.size() == 1 && std::holds_alternative<std::vector<eval::Value>>(args[0].value))
    {
        return find_min(std::get<std::vector<eval::Value>>(args[0].value));
    }
    return find_min(args);
};

} // namespace CuraFormulaeEngine::env
