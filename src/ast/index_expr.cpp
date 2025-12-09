#include "cura-formulae-engine/ast/index_expr.h"

#include "cura-formulae-engine/ast/ast.h"
#include "cura-formulae-engine/ast/expr_ptr.h"

#include <fmt/format.h>
#include <zeus/expected.hpp>

#include <string>
#include <unordered_set>
#include <variant>

namespace CuraFormulaeEngine::ast
{

[[nodiscard]] std::string IndexExpr::toString() const noexcept
{
    return fmt::format("{}[{}]", array.toString(), index.toString());
}

[[nodiscard]] eval::Result IndexExpr::evaluate(const env::Environment* environment) const noexcept
{
    const auto array_result = array.evaluate(environment);
    if (! array_result.has_value())
    {
        return zeus::unexpected(array_result.error());
    }
    const auto& array_value = array_result.value();

    const auto index_result = index.evaluate(environment);
    if (! index_result.has_value())
    {
        return zeus::unexpected(index_result.error());
    }
    const auto& index_value = index_result.value();

    return array_value[index_value];
}

[[nodiscard]] std::unordered_set<std::string> IndexExpr::freeVariables() const noexcept
{
    auto array_vars = array.freeVariables();
    auto index_vars = index.freeVariables();
    array_vars.insert(index_vars.begin(), index_vars.end());
    return array_vars;
}

[[nodiscard]] bool IndexExpr::deepEq(const Expr& other) const noexcept
{
    if (const auto other_index_expr = dynamic_cast<const IndexExpr*>(&other))
    {
        return array.deepEq(other_index_expr->array) && index.deepEq(other_index_expr->index);
    }
    return false;
}

void IndexExpr::visitAll(std::function<void(const Expr&)> visitor) const noexcept
{
    visitor(*this);
    array.visitAll(visitor);
    index.visitAll(visitor);
}

} // namespace CuraFormulaeEngine::ast
