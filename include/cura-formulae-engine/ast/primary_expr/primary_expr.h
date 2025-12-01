#pragma once

#include "cura-formulae-engine/ast/ast.h"
#include "cura-formulae-engine/eval.h"

#include <fmt/format.h>

#include <functional>
#include <string>
#include <unordered_set>

namespace CuraFormulaeEngine::ast
{

template<typename T>
struct PrimaryExpr : public Expr
{
    T value;

    explicit PrimaryExpr(T value)
        : value(value)
    {
    }

    [[nodiscard]] eval::Result evaluate(const env::Environment*) const noexcept final
    {
        return value;
    }

    [[nodiscard]] std::unordered_set<std::string> freeVariables() const noexcept final
    {
        return {};
    };

    [[nodiscard]] std::string toString() const noexcept override
    {
        return fmt::format("{}", value);
    }

    [[nodiscard]] bool deepEq(const Expr& other) const noexcept final
    {
        if (auto other_number = dynamic_cast<const PrimaryExpr<T>*>(&other))
        {
            return value == other_number->value;
        }
        return false;
    }

    void visitAll(std::function<void(const Expr&)> visitor) const noexcept final
    {
        visitor(*this);
    }
};

} // namespace CuraFormulaeEngine::ast
