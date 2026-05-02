#pragma once

#include <cstdint>
#include <functional>

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
#include <unordered_map>
#include <variant>
#include <vector>
#include <zeus/expected.hpp>
#include <spdlog/spdlog.h>

namespace CuraFormulaeEngine::eval
{

struct Value;

enum class Error
{
    TypeMismatch,
    UndefinedVariable,
    DivisionByZero,
    InvalidNumberOfArguments,
    IndexOutOfBounds,
    ValueError
};

using Result = zeus::expected<Value, Error>;

struct Value
{
    using fn_t = std::function<Result(const std::vector<Value>&)>;

    struct rich_fn_t
    {
        fn_t operation;
        std::vector<std::string> signature;

        [[nodiscard]] Result operator()(const std::vector<Value>& args) const noexcept
        {
            if (! operation)
            {
                return zeus::unexpected(Error::TypeMismatch);
            }
            return operation(args);
        }

        [[nodiscard]] std::optional<std::vector<std::string>> getSignature() const noexcept
        {
            if (signature.empty())
            {
                return std::nullopt;
            }
            return signature;
        }
    };

    std::variant<bool, double, std::int64_t, std::string, std::vector<Value>, fn_t, rich_fn_t, std::unordered_map<std::string, Value>, std::nullptr_t> value = nullptr;

    Value() noexcept = default;

    Value(const bool& value) noexcept
        : value{ value }
    {
    }

    Value(const double& value) noexcept
        : value{ value }
    {
    }

    Value(const std::int64_t& value) noexcept
        : value{ value }
    {
    }

    Value(const std::string& value) noexcept
        : value{ value }
    {
    }

    Value(const std::vector<Value>& value) noexcept
        : value{ value }
    {
    }

    Value(const fn_t& value) noexcept
        : value{ value }
    {
    }

    Value(const rich_fn_t& value) noexcept
        : value{ value }
    {
    }

    Value(const std::unordered_map<std::string, Value>& value) noexcept
        : value{ value }
    {
    }

    Value(const std::nullptr_t& value) noexcept
        : value{ value }
    {
    }

    static Value none() noexcept
    {
        return Value(nullptr);
    }

    ~Value() = default;

    Value(Value&& other) noexcept = default;
    Value(const Value& other) noexcept = default;
    Value& operator=(const Value& other) noexcept = default;
    Value& operator=(Value&& other) noexcept = default;

    [[nodiscard]] std::string toString() const noexcept;

    [[nodiscard]] bool deepEq(const Value& other) const noexcept;

    [[nodiscard]] bool isTruthy() const noexcept;

    [[nodiscard]] zeus::expected<double, Error> numeric() const noexcept;

#ifdef EMSCRIPTEN
    [[nodiscard]] emscripten::val toEmscripten() const;
#endif

    Result operator[](const Value&) const noexcept;
};

/**
 * @brief Utility function to try to get a value of a specific type from an
 * eval_result. If the eval_result is an error, the error is propagated.
 *
 * @tparam ExpectedType
 * @param result
 * @return
 */
template<typename ExpectedType>
zeus::expected<ExpectedType, Error> try_get(const Result& result)
{
    if (! result.has_value())
    {
        return zeus::unexpected(result.error());
    }

    const auto& type_result = result.value();
    if (! std::holds_alternative<ExpectedType>(type_result.value))
    {
        return zeus::unexpected(Error::TypeMismatch);
    }

    return std::get<ExpectedType>(type_result.value);
}

Result pow(const Value& lhs, const Value& rhs);

} // namespace CuraFormulaeEngine::eval

bool operator==(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept;
bool operator!=(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept;
zeus::expected<bool, CuraFormulaeEngine::eval::Error> operator<(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept;
zeus::expected<bool, CuraFormulaeEngine::eval::Error> operator<=(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept;
zeus::expected<bool, CuraFormulaeEngine::eval::Error> operator>=(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept;
zeus::expected<bool, CuraFormulaeEngine::eval::Error> operator>(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept;
zeus::expected<bool, CuraFormulaeEngine::eval::Error> operator&&(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept;
zeus::expected<bool, CuraFormulaeEngine::eval::Error> operator||(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept;
CuraFormulaeEngine::eval::Result operator+(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept;
CuraFormulaeEngine::eval::Result operator-(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept;
CuraFormulaeEngine::eval::Result operator*(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept;
CuraFormulaeEngine::eval::Result operator/(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept;
CuraFormulaeEngine::eval::Result operator%(const CuraFormulaeEngine::eval::Value& lhs, const CuraFormulaeEngine::eval::Value& rhs) noexcept;
CuraFormulaeEngine::eval::Result operator!(const CuraFormulaeEngine::eval::Value& operand) noexcept;
CuraFormulaeEngine::eval::Result operator-(const CuraFormulaeEngine::eval::Value& operand) noexcept;
