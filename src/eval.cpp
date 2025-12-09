#include "cura-formulae-engine/eval.h"

#include <cstdint>

#ifdef EMSCRIPTEN
#include <emscripten/val.h>
#endif
#include <fmt/core.h>
#include <range/v3/algorithm/all_of.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/c_str.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>

#include <cstddef>
#include <functional>
#include <string>
#include <variant>
#include <vector>
#include <zeus/expected.hpp>

namespace CuraFormulaeEngine::eval {

    [[nodiscard]] std::string Value::toString() const noexcept
    {
        if (std::holds_alternative<bool>(value))
        {
            return std::get<bool>(value) ? "true" : "false";
        }
        if (std::holds_alternative<double>(value))
        {
            auto str = std::to_string(std::get<double>(value));
            str.erase(str.find_last_not_of('0') + 1, std::string::npos);
            str.erase(str.find_last_not_of('.') + 1, std::string::npos);
            return str;
        }
        if (std::holds_alternative<std::int64_t>(value))
        {
            return std::to_string(std::get<std::int64_t>(value));
        }
        if (std::holds_alternative<std::string>(value))
        {
            return fmt::format("{}", std::get<std::string>(value));
        }
        if (std::holds_alternative<std::vector<Value>>(value))
        {
            auto list = std::get<std::vector<Value>>(value);
            return fmt::format(
                    "[{}]",
                    list | ranges::views::transform([](const auto &element) { return element.toString(); }) |
                    ranges::views::join(ranges::views::c_str(", "))
                    | ranges::to<std::string>());
        }
        if (std::holds_alternative<std::nullptr_t>(value))
        {
            return "None";
        }
        if (std::holds_alternative<fn_t>(value))
        {
            return "<function>";
        }
        return "<unknown>";
    }

    [[nodiscard]] bool Value::deepEq(const Value &other) const noexcept
    {
        if (std::holds_alternative<bool>(value) && std::holds_alternative<bool>(other.value))
        {
            return std::get<bool>(value) == std::get<bool>(other.value);
        }
        if (std::holds_alternative<double>(value) && std::holds_alternative<double>(other.value))
        {
            return std::get<double>(value) == std::get<double>(other.value);
        }
        if (std::holds_alternative<std::int64_t>(value) && std::holds_alternative<std::int64_t>(other.value))
        {
            return std::get<std::int64_t>(value) == std::get<std::int64_t>(other.value);
        }
        if (std::holds_alternative<std::string>(value) && std::holds_alternative<std::string>(other.value))
        {
            return std::get<std::string>(value) == std::get<std::string>(other.value);
        }
        if (std::holds_alternative<std::nullptr_t>(value) && std::holds_alternative<std::nullptr_t>(other.value))
        {
            return true;
        }
        if (std::holds_alternative<std::vector<Value>>(value) && std::holds_alternative<std::vector<Value>>(other.value))
        {
            const auto &lhs = std::get<std::vector<Value>>(value);
            const auto &rhs = std::get<std::vector<Value>>(other.value);
            if (lhs.size() != rhs.size())
            {
                return false;
            }
            return ranges::all_of(
                    ranges::views::zip(lhs, rhs),
                    [](const auto &pair)
                    {
                        const auto [element_lhs, element_rhs] = pair;
                        return element_lhs.deepEq(element_rhs);
                    });
        }
        if (std::holds_alternative<fn_t>(value) && std::holds_alternative<fn_t>(other.value))
        {
            return typeid(std::holds_alternative<fn_t>(value)) == typeid(std::holds_alternative<fn_t>(other.value));
        }
        return false;
    }

    [[nodiscard]] bool Value::isTruthy() const noexcept
    {
        if (std::holds_alternative<bool>(value))
        {
            return std::get<bool>(value);
        }
        if (std::holds_alternative<double>(value))
        {
            return std::get<double>(value) != 0.0;
        }
        if (std::holds_alternative<std::int64_t>(value))
        {
            return std::get<std::int64_t>(value) != 0L;
        }
        if (std::holds_alternative<std::string>(value))
        {
            return ! std::get<std::string>(value).empty();
        }
        if (std::holds_alternative<std::vector<Value>>(value))
        {
            return ! std::get<std::vector<Value>>(value).empty();
        }
        return false;
    };

    [[nodiscard]] zeus::expected<double, Error> Value::numeric() const noexcept
    {
        if (std::holds_alternative<double>(value))
        {
            return std::get<double>(value);
        }
        if (std::holds_alternative<std::int64_t>(value))
        {
            return static_cast<double>(std::get<std::int64_t>(value));
        }

        if (std::holds_alternative<bool>(value))
        {
            return std::get<bool>(value) ? 1.0 : 0.0;
        }
        return zeus::unexpected(Error::TypeMismatch);
    }

#ifdef EMSCRIPTEN
    [[nodiscard]] emscripten::val Value::toEmscripten() const
    {
        if (std::holds_alternative<bool>(value))
        {
            return emscripten::val(std::get<bool>(value));
        }
        if (std::holds_alternative<double>(value))
        {
            return emscripten::val(std::get<double>(value));
        }
        if (std::holds_alternative<std::int64_t>(value))
        {
            return emscripten::val(static_cast<double>(std::get<std::int64_t>(value)));
        }
        if (std::holds_alternative<std::string>(value))
        {
            return emscripten::val(std::get<std::string>(value));
        }
        if (std::holds_alternative<std::nullptr_t>(value))
        {
            return emscripten::val::null();
        }
        if (std::holds_alternative<std::vector<Value>>(value))
        {
            emscripten::val array = emscripten::val::array();
            const auto& vector = std::get<std::vector<Value>>(value);
            for (size_t i = 0; i < vector.size(); ++i)
            {
                array.set(i, vector[i].toEmscripten());
            }
            return array;
        }
        if (std::holds_alternative<fn_t>(value))
        {
            throw std::runtime_error("Cannot convert function to emscripten");
        }

        throw std::runtime_error("Unknown type in `Value::toEmscripten`");
    }
#endif

    Result pow(const Value& lhs, const Value& rhs)
    {
        if (std::holds_alternative<double>(lhs.value) && std::holds_alternative<double>(rhs.value))
        {
            return std::pow(std::get<double>(lhs.value), std::get<double>(rhs.value));
        }
        if (std::holds_alternative<std::int64_t>(lhs.value) && std::holds_alternative<std::int64_t>(rhs.value))
        {
            return static_cast<std::int64_t>(std::pow(std::get<std::int64_t>(lhs.value), std::get<std::int64_t>(rhs.value)));
        }
        if ((std::holds_alternative<std::int64_t>(lhs.value) || std::holds_alternative<double>(lhs.value))
            && (std::holds_alternative<std::int64_t>(rhs.value) || std::holds_alternative<double>(rhs.value)))
        {
            const auto lhs_value = std::holds_alternative<double>(lhs.value) ? std::get<double>(lhs.value) : static_cast<double>(std::get<std::int64_t>(lhs.value));
            const auto rhs_value = std::holds_alternative<double>(rhs.value) ? std::get<double>(rhs.value) : static_cast<double>(std::get<std::int64_t>(rhs.value));
            return std::pow(lhs_value, rhs_value);
        }
        return zeus::unexpected(Error::TypeMismatch);
    }

} // namespace CuraFormulaeEngine::eval

bool operator==(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept
{
    if (std::holds_alternative<bool>(lhs.value) && std::holds_alternative<bool>(rhs.value))
    {
        return std::get<bool>(lhs.value) == std::get<bool>(rhs.value);
    }
    if (std::holds_alternative<double>(lhs.value) && std::holds_alternative<double>(rhs.value))
    {
        return std::get<double>(lhs.value) == std::get<double>(rhs.value);
    }
    if (std::holds_alternative<std::int64_t>(lhs.value) && std::holds_alternative<std::int64_t>(rhs.value))
    {
        return std::get<std::int64_t>(lhs.value) == std::get<std::int64_t>(rhs.value);
    }
    if (std::holds_alternative<double>(lhs.value) && std::holds_alternative<std::int64_t>(rhs.value))
    {
        return std::get<double>(lhs.value) == static_cast<double>(std::get<std::int64_t>(rhs.value));
    }
    if (std::holds_alternative<std::int64_t>(lhs.value) && std::holds_alternative<double>(rhs.value))
    {
        return static_cast<double>(std::get<std::int64_t>(lhs.value)) == std::get<double>(rhs.value);
    }
    if (std::holds_alternative<bool>(lhs.value) && std::holds_alternative<std::int64_t>(rhs.value))
    {
        return (std::get<bool>(lhs.value) ? 1 : 0) == std::get<std::int64_t>(rhs.value);
    }
    if (std::holds_alternative<std::int64_t>(lhs.value) && std::holds_alternative<bool>(rhs.value))
    {
        return std::get<std::int64_t>(lhs.value) == (std::get<bool>(rhs.value) ? 1 : 0);
    }
    if (std::holds_alternative<bool>(lhs.value) && std::holds_alternative<double>(rhs.value))
    {
        return (std::get<bool>(lhs.value) ? 1.0 : 0.0) == std::get<double>(rhs.value);
    }
    if (std::holds_alternative<double>(lhs.value) && std::holds_alternative<bool>(rhs.value))
    {
        return std::get<double>(lhs.value) == (std::get<bool>(rhs.value) ? 1.0 : 0.0);
    }
    if (std::holds_alternative<std::string>(lhs.value) && std::holds_alternative<std::string>(rhs.value))
    {
        return std::get<std::string>(lhs.value) == std::get<std::string>(rhs.value);
    }
    if (std::holds_alternative<std::vector<CuraFormulaeEngine::eval::Value>>(lhs.value) && std::holds_alternative<std::vector<CuraFormulaeEngine::eval::Value>>(rhs.value))
    {
        auto vector_lhs = std::get<std::vector<CuraFormulaeEngine::eval::Value>>(lhs.value);
        auto vector_rhs = std::get<std::vector<CuraFormulaeEngine::eval::Value>>(rhs.value);

        if (vector_lhs.size() != vector_rhs.size())
        {
            return false;
        }

        return ranges::all_of(
                ranges::views::zip(vector_lhs, vector_rhs),
                [](const auto& pair)
                {
                    const auto [element_lhs, element_rhs] = pair;
                    return element_lhs == element_rhs;
                });
    }

    return false;
}

bool operator!=(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept
{
    return ! (lhs == rhs);
}

zeus::expected<bool, CuraFormulaeEngine::eval::Error> operator<(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept
{
    const auto lhs_numeric = lhs.numeric();
    const auto rhs_numeric = rhs.numeric();
    if (lhs_numeric.has_value() && rhs_numeric.has_value())
    {
        return lhs_numeric.value() < rhs_numeric.value();
    }
    if (std::holds_alternative<std::string>(lhs.value) && std::holds_alternative<std::string>(rhs.value))
    {
        return std::get<std::string>(lhs.value) < std::get<std::string>(rhs.value);
    }
    return zeus::unexpected(CuraFormulaeEngine::eval::Error::TypeMismatch);
}

zeus::expected<bool, CuraFormulaeEngine::eval::Error> operator<=(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept
{
    const auto lhs_numeric = lhs.numeric();
    const auto rhs_numeric = rhs.numeric();
    if (lhs_numeric.has_value() && rhs_numeric.has_value())
    {
        return lhs_numeric.value() <= rhs_numeric.value();
    }
    return zeus::unexpected(CuraFormulaeEngine::eval::Error::TypeMismatch);
}

zeus::expected<bool, CuraFormulaeEngine::eval::Error> operator>=(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept
{
    const auto lhs_numeric = lhs.numeric();
    const auto rhs_numeric = rhs.numeric();
    if (lhs_numeric.has_value() && rhs_numeric.has_value())
    {
        return lhs_numeric.value() >= rhs_numeric.value();
    }
    if (std::holds_alternative<std::string>(lhs.value) && std::holds_alternative<std::string>(rhs.value))
    {
        return std::get<std::string>(lhs.value) >= std::get<std::string>(rhs.value);
    }
    return zeus::unexpected(CuraFormulaeEngine::eval::Error::TypeMismatch);
}

zeus::expected<bool, CuraFormulaeEngine::eval::Error> operator>(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept
{
    const auto lhs_numeric = lhs.numeric();
    const auto rhs_numeric = rhs.numeric();
    if (lhs_numeric.has_value() && rhs_numeric.has_value())
    {
        return lhs_numeric.value() > rhs_numeric.value();
    }
    if (std::holds_alternative<std::string>(lhs.value) && std::holds_alternative<std::string>(rhs.value))
    {
        return std::get<std::string>(lhs.value) > std::get<std::string>(rhs.value);
    }
    return zeus::unexpected(CuraFormulaeEngine::eval::Error::TypeMismatch);
}

zeus::expected<bool, CuraFormulaeEngine::eval::Error> operator&&(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept
{
    if (std::holds_alternative<bool>(lhs.value) && std::holds_alternative<bool>(rhs.value))
    {
        return std::get<bool>(lhs.value) && std::get<bool>(rhs.value);
    }
    return zeus::unexpected(CuraFormulaeEngine::eval::Error::TypeMismatch);
}

zeus::expected<bool, CuraFormulaeEngine::eval::Error> operator||(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept
{
    if (std::holds_alternative<bool>(lhs.value) && std::holds_alternative<bool>(rhs.value))
    {
        return std::get<bool>(lhs.value) || std::get<bool>(rhs.value);
    }
    return zeus::unexpected(CuraFormulaeEngine::eval::Error::TypeMismatch);
}

CuraFormulaeEngine::eval::Result operator+(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept
{
    if (std::holds_alternative<std::string>(lhs.value) && std::holds_alternative<std::string>(rhs.value))
    {
        return std::get<std::string>(lhs.value) + std::get<std::string>(rhs.value);
    }
    if ((std::holds_alternative<bool>(lhs.value) || std::holds_alternative<std::int64_t>(lhs.value))
        && (std::holds_alternative<bool>(rhs.value) || std::holds_alternative<std::int64_t>(rhs.value)))
    {
        const auto lhs_value = std::holds_alternative<std::int64_t>(lhs.value) ? std::get<std::int64_t>(lhs.value) : static_cast<std::int64_t>(std::get<bool>(lhs.value));
        const auto rhs_value = std::holds_alternative<std::int64_t>(rhs.value) ? std::get<std::int64_t>(rhs.value) : static_cast<std::int64_t>(std::get<bool>(rhs.value));
        return lhs_value + rhs_value;
    }
    if ((std::holds_alternative<bool>(lhs.value) || std::holds_alternative<double>(lhs.value))
        && (std::holds_alternative<bool>(rhs.value) || std::holds_alternative<double>(rhs.value)))
    {
        const auto lhs_value = std::holds_alternative<double>(lhs.value) ? std::get<double>(lhs.value) : static_cast<double>(std::get<bool>(lhs.value));
        const auto rhs_value = std::holds_alternative<double>(rhs.value) ? std::get<double>(rhs.value) : static_cast<double>(std::get<bool>(rhs.value));
        return lhs_value + rhs_value;
    }
    if ((std::holds_alternative<double>(lhs.value) || std::holds_alternative<std::int64_t>(lhs.value))
        && (std::holds_alternative<double>(rhs.value) || std::holds_alternative<std::int64_t>(rhs.value)))
    {
        const auto lhs_value = std::holds_alternative<double>(lhs.value) ? std::get<double>(lhs.value) : static_cast<double>(std::get<std::int64_t>(lhs.value));
        const auto rhs_value = std::holds_alternative<double>(rhs.value) ? std::get<double>(rhs.value) : static_cast<double>(std::get<std::int64_t>(rhs.value));
        return lhs_value + rhs_value;
    }
    if (std::holds_alternative<std::vector<CuraFormulaeEngine::eval::Value>>(lhs.value) && std::holds_alternative<std::vector<CuraFormulaeEngine::eval::Value>>(rhs.value))
    {
        auto list_lhs = std::get<std::vector<CuraFormulaeEngine::eval::Value>>(lhs.value);
        auto list_rhs = std::get<std::vector<CuraFormulaeEngine::eval::Value>>(rhs.value);
        list_lhs.insert(list_lhs.end(), list_rhs.begin(), list_rhs.end());
        return list_lhs;
    }
    return zeus::unexpected(CuraFormulaeEngine::eval::Error::TypeMismatch);
}

CuraFormulaeEngine::eval::Result operator-(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept
{
    if ((std::holds_alternative<bool>(lhs.value) || std::holds_alternative<std::int64_t>(lhs.value))
        && (std::holds_alternative<bool>(rhs.value) || std::holds_alternative<std::int64_t>(rhs.value)))
    {
        const auto lhs_value = std::holds_alternative<std::int64_t>(lhs.value) ? std::get<std::int64_t>(lhs.value) : static_cast<std::int64_t>(std::get<bool>(lhs.value));
        const auto rhs_value = std::holds_alternative<std::int64_t>(rhs.value) ? std::get<std::int64_t>(rhs.value) : static_cast<std::int64_t>(std::get<bool>(rhs.value));
        return lhs_value - rhs_value;
    }
    if ((std::holds_alternative<bool>(lhs.value) || std::holds_alternative<double>(lhs.value))
        && (std::holds_alternative<bool>(rhs.value) || std::holds_alternative<double>(rhs.value)))
    {
        const auto lhs_value = std::holds_alternative<double>(lhs.value) ? std::get<double>(lhs.value) : static_cast<double>(std::get<bool>(lhs.value));
        const auto rhs_value = std::holds_alternative<double>(rhs.value) ? std::get<double>(rhs.value) : static_cast<double >(std::get<bool>(rhs.value));
        return lhs_value - rhs_value;
    }
    if ((std::holds_alternative<double>(lhs.value) || std::holds_alternative<std::int64_t>(lhs.value))
        && (std::holds_alternative<double>(rhs.value) || std::holds_alternative<std::int64_t>(rhs.value)))
    {
        const auto lhs_value = std::holds_alternative<double>(lhs.value) ? std::get<double>(lhs.value) : static_cast<double>(std::get<std::int64_t>(lhs.value));
        const auto rhs_value = std::holds_alternative<double>(rhs.value) ? std::get<double>(rhs.value) : static_cast<double>(std::get<std::int64_t>(rhs.value));
        return lhs_value - rhs_value;
    }
    return zeus::unexpected(CuraFormulaeEngine::eval::Error::TypeMismatch);
}

CuraFormulaeEngine::eval::Result operator*(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept
{
    if (std::holds_alternative<std::int64_t>(lhs.value) && std::holds_alternative<std::int64_t>(rhs.value))
    {
        return std::get<std::int64_t>(lhs.value) * std::get<std::int64_t>(rhs.value);
    }
    if ((std::holds_alternative<std::int64_t>(lhs.value) || std::holds_alternative<double>(lhs.value))
        && (std::holds_alternative<std::int64_t>(rhs.value) || std::holds_alternative<double>(rhs.value)))
    {
        const auto lhs_value = std::holds_alternative<double>(lhs.value) ? std::get<double>(lhs.value) : static_cast<double>(std::get<std::int64_t>(lhs.value));
        const auto rhs_value = std::holds_alternative<double>(rhs.value) ? std::get<double>(rhs.value) : static_cast<double>(std::get<std::int64_t>(rhs.value));
        return lhs_value * rhs_value;
    }
    if ((std::holds_alternative<bool>(lhs.value) || std::holds_alternative<double>(lhs.value))
        && (std::holds_alternative<bool>(rhs.value) || std::holds_alternative<double>(rhs.value)))
    {
        const auto lhs_value = std::holds_alternative<double>(lhs.value) ? std::get<double>(lhs.value) : static_cast<double>(std::get<bool>(lhs.value));
        const auto rhs_value = std::holds_alternative<double>(rhs.value) ? std::get<double>(rhs.value) : static_cast<double >(std::get<bool>(rhs.value));
        return lhs_value * rhs_value;
    }
    if ((std::holds_alternative<std::string>(lhs.value) && std::holds_alternative<std::int64_t>(rhs.value))
        || (std::holds_alternative<std::int64_t>(lhs.value) && std::holds_alternative<std::string>(rhs.value)))
    {
        const auto& string = std::holds_alternative<std::string>(lhs.value) ? std::get<std::string>(lhs.value) : std::get<std::string>(rhs.value);
        const auto& times = std::holds_alternative<std::int64_t>(lhs.value) ? std::get<std::int64_t>(lhs.value) : std::get<std::int64_t>(rhs.value);
        std::string result;
        for (std::int64_t i = 0; i < times; ++i)
        {
            result += string;
        }
        return result;
    }
    if ((std::holds_alternative<std::vector<CuraFormulaeEngine::eval::Value>>(lhs.value) && std::holds_alternative<std::int64_t>(rhs.value))
        || (std::holds_alternative<std::int64_t>(lhs.value) && std::holds_alternative<std::vector<CuraFormulaeEngine::eval::Value>>(rhs.value)))
    {
        const auto& string = std::holds_alternative<std::vector<CuraFormulaeEngine::eval::Value>>(lhs.value) ? std::get<std::vector<CuraFormulaeEngine::eval::Value>>(lhs.value)
                                                                                                  : std::get<std::vector<CuraFormulaeEngine::eval::Value>>(rhs.value);
        const auto& times = std::holds_alternative<std::int64_t>(lhs.value) ? std::get<std::int64_t>(lhs.value) : std::get<std::int64_t>(rhs.value);
        std::vector<CuraFormulaeEngine::eval::Value> result;
        for (std::int64_t i = 0; i < times; ++i)
        {
            result.insert(result.end(), string.begin(), string.end());
        }
        return result;
    }
    return zeus::unexpected(CuraFormulaeEngine::eval::Error::TypeMismatch);
}

CuraFormulaeEngine::eval::Result operator/(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept
{
    if (std::holds_alternative<std::int64_t>(lhs.value) && std::holds_alternative<std::int64_t>(rhs.value))
    {
        if (std::get<std::int64_t>(rhs.value) == 0L)
        {
            return zeus::unexpected(CuraFormulaeEngine::eval::Error::DivisionByZero);
        }
        return static_cast<double>(std::get<std::int64_t>(lhs.value)) / static_cast<double>(std::get<std::int64_t>(rhs.value));
    }
    if ((std::holds_alternative<std::int64_t>(lhs.value) || std::holds_alternative<double>(lhs.value))
        && (std::holds_alternative<std::int64_t>(rhs.value) || std::holds_alternative<double>(rhs.value)))
    {
        const auto lhs_value = std::holds_alternative<double>(lhs.value) ? std::get<double>(lhs.value) : static_cast<double>(std::get<std::int64_t>(lhs.value));
        const auto rhs_value = std::holds_alternative<double>(rhs.value) ? std::get<double>(rhs.value) : static_cast<double>(std::get<std::int64_t>(rhs.value));

        if (rhs_value == 0.0)
        {
            return zeus::unexpected(CuraFormulaeEngine::eval::Error::DivisionByZero);
        }
        return lhs_value / rhs_value;
    }
    return zeus::unexpected(CuraFormulaeEngine::eval::Error::TypeMismatch);
}

CuraFormulaeEngine::eval::Result operator%(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept
{
    if (std::holds_alternative<double>(lhs.value) && std::holds_alternative<double>(rhs.value))
    {
        return std::fmod(std::get<double>(lhs.value), std::get<double>(rhs.value));
    }
    if (std::holds_alternative<std::int64_t>(lhs.value) && std::holds_alternative<std::int64_t>(rhs.value))
    {
        return std::get<std::int64_t>(lhs.value) % std::get<std::int64_t>(rhs.value);
    }
    return zeus::unexpected(CuraFormulaeEngine::eval::Error::TypeMismatch);
}

CuraFormulaeEngine::eval::Result CuraFormulaeEngine::eval::Value::operator[](const Value& index) const noexcept
{
    if (! std::holds_alternative<std::vector<Value>>(this->value))
    {
        return zeus::unexpected(CuraFormulaeEngine::eval::Error::TypeMismatch);
    }
    const auto& list = std::get<std::vector<Value>>(this->value);

    if (! std::holds_alternative<std::int64_t>(index.value))
    {
        return zeus::unexpected(CuraFormulaeEngine::eval::Error::TypeMismatch);
    }
    const auto& index_value = std::get<std::int64_t>(index.value);

    std::int64_t index_value_absolute;
    if (index_value < 0)
    {
        index_value_absolute = static_cast<std::int64_t>(list.size()) - -index_value;
        if (index_value_absolute < 0)
        {
            return zeus::unexpected(eval::Error::IndexOutOfBounds);
        }
    }
    else
    {
        index_value_absolute = index_value;
        if (index_value_absolute >= static_cast<std::int64_t>(list.size()))
        {
            return zeus::unexpected(eval::Error::IndexOutOfBounds);
        }
    }

    return list[static_cast<std::size_t>(index_value_absolute)];
}

CuraFormulaeEngine::eval::Result operator!(const CuraFormulaeEngine::eval::Value& operand) noexcept
{
    if (std::holds_alternative<std::vector<CuraFormulaeEngine::eval::Value>>(operand.value))
    {
        return std::get<std::vector<CuraFormulaeEngine::eval::Value>>(operand.value).empty();
    }
    if (std::holds_alternative<std::int64_t>(operand.value))
    {
        return std::get<std::int64_t>(operand.value) == 0L;
    }
    if (std::holds_alternative<double>(operand.value))
    {
        return std::get<double>(operand.value) == 0.0;
    }
    if (std::holds_alternative<std::string>(operand.value))
    {
        return std::get<std::string>(operand.value) == std::string("");
    }
    if (std::holds_alternative<bool>(operand.value))
    {
        return ! std::get<bool>(operand.value);
    }
    return zeus::unexpected(CuraFormulaeEngine::eval::Error::TypeMismatch);
}

CuraFormulaeEngine::eval::Result operator-(const CuraFormulaeEngine::eval::Value& operand) noexcept
{
    if (std::holds_alternative<bool>(operand.value))
    {
        return std::get<bool>(operand.value) ? static_cast<std::int64_t>(-1) : static_cast<std::int64_t>(0);
    }
    if (std::holds_alternative<std::int64_t>(operand.value))
    {
        return -std::get<std::int64_t>(operand.value);
    }
    if (std::holds_alternative<double>(operand.value))
    {
        return -std::get<double>(operand.value);
    }
    return zeus::unexpected(CuraFormulaeEngine::eval::Error::TypeMismatch);
}
