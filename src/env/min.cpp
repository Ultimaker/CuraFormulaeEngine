#include "cura-formulae-engine/env/min.h"

#include <range/v3/view/drop.hpp>
#include <zeus/expected.hpp>

#include <variant>
#include <vector>

namespace CuraFormulaeEngine::env
{

[[nodiscard]] eval::Result MinFunction::operator()(const std::vector<eval::Value> &args) const noexcept
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

    const auto find_min_with_key = [](const std::vector<eval::Value>& vec, const eval::Value& key_fn) -> eval::Result
    {
        if (vec.empty())
        {
            return zeus::unexpected(eval::Error::InvalidNumberOfArguments);
        }

        eval::Value::fn_t fn;
        if (std::holds_alternative<eval::Value::fn_t>(key_fn.value))
        {
            fn = std::get<eval::Value::fn_t>(key_fn.value);
        }
        else if (std::holds_alternative<eval::Value::rich_fn_t>(key_fn.value))
        {
            fn = std::get<eval::Value::rich_fn_t>(key_fn.value).operation;
        }
        else
        {
            return zeus::unexpected(eval::Error::TypeMismatch);
        }

        auto min_elem = vec[0];
        auto min_key = fn(std::vector<eval::Value>{min_elem});
        if (! min_key.has_value())
        {
            return zeus::unexpected(min_key.error());
        }

        for (const auto& elem : vec | ranges::views::drop(1))
        {
            auto elem_key = fn(std::vector<eval::Value>{elem});
            if (! elem_key.has_value())
            {
                return zeus::unexpected(elem_key.error());
            }

            const auto cmp = elem_key.value() < min_key.value();
            if (! cmp.has_value())
            {
                return zeus::unexpected(eval::Error::TypeMismatch);
            }

            if (cmp.value())
            {
                min_elem = elem;
                min_key = elem_key;
            }
        }
        return min_elem;
    };

    if (args.size() == 1 && std::holds_alternative<std::vector<eval::Value>>(args[0].value))
    {
        return find_min(std::get<std::vector<eval::Value>>(args[0].value));
    }
    
    if (args.size() == 2)
    {
        // key parameter provided
        if (std::holds_alternative<std::vector<eval::Value>>(args[0].value))
        {
            return find_min_with_key(std::get<std::vector<eval::Value>>(args[0].value), args[1]);
        }
        else
        {
            // Single iterable as first arg
            return find_min_with_key(args, args[1]);
        }
    }

    // Multiple arguments: find min among them (no key)
    return find_min(args);
}

[[nodiscard]] std::vector<std::string> MinFunction::getSignature() const noexcept
{
    return { "iterable", "key" };
}

const MinFunction min_function{};
const eval::Value::fn_t min = [](const std::vector<eval::Value>& args) -> eval::Result
{
    return min_function(args);
};

} // namespace CuraFormulaeEngine::env
