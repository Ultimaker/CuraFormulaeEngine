#include "cura-formulae-engine/ast/variable_expr.h"
#include "cura-formulae-engine/ast/ast.h"
#include "cura-formulae-engine/eval.h"

#include <zeus/expected.hpp>

#include <string>
#include <unordered_set>

namespace CuraFormulaeEngine::ast
{

std::string VariableExpr::toString() const noexcept
{
    return name;
}

eval::Result VariableExpr::evaluate(const env::Environment* environment) const noexcept
{
    if (const auto value = environment->get(name); value.has_value())
    {
        return value.value();
    }
    return zeus::unexpected(eval::Error::UndefinedVariable);
}

std::unordered_set<std::string> VariableExpr::freeVariables() const noexcept
{
    return { name };
}

bool VariableExpr::deepEq(const Expr& other) const noexcept
{
    if (const auto* other_variable = dynamic_cast<const VariableExpr*>(&other))
    {
        return name == other_variable->name;
    }
    return false;
}

void VariableExpr::visitAll(std::function<void(const Expr&)> visitor) const noexcept
{
    visitor(*this);
}

} // namespace CuraFormulaeEngine::ast
