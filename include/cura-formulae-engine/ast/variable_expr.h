#pragma once

#include "cura-formulae-engine/ast/ast.h"
#include "cura-formulae-engine/env/env.h"
#include "cura-formulae-engine/eval.h"

#include <zeus/expected.hpp>

namespace CuraFormulaeEngine::ast
{

struct VariableExpr final : Expr
{
    std::string name;

    VariableExpr(std::string name)
        : name(std::move(name))
    {
    }

    std::string toString() const noexcept final;

    eval::Result evaluate(const env::Environment* environment) const noexcept final;

    std::unordered_set<std::string> freeVariables() const noexcept final;

    bool deepEq(const Expr& other) const noexcept final;

    void visitAll(std::function<void(const Expr&)> visitor) const noexcept final;
};

} // namespace CuraFormulaeEngine::ast
