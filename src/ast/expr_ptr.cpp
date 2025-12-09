#include "cura-formulae-engine/ast/expr_ptr.h"
#include "cura-formulae-engine/ast/index_expr.h"

#include <string>
#include <unordered_set>

namespace CuraFormulaeEngine::ast
{

eval::Result ExprPtr::evaluate(const env::Environment* environment) const noexcept
{
    return ptr->evaluate(environment);
}

std::unordered_set<std::string> ExprPtr::freeVariables() const noexcept
{
    return ptr->freeVariables();
}

std::string ExprPtr::toString() const noexcept
{
    return ptr->toString();
}

bool ExprPtr::deepEq(const Expr& other) const noexcept
{
    if (const auto& other_ptr = dynamic_cast<const ExprPtr*>(&other))
    {
        return ptr->deepEq(*other_ptr->ptr);
    }
    return false;
}

void ExprPtr::visitAll(std::function<void(const Expr&)> visitor) const noexcept
{
    visitor(*this);
    ptr->visitAll(visitor);
}

} // namespace CuraFormulaeEngine::ast

CuraFormulaeEngine::ast::ExprPtr CuraFormulaeEngine::ast::ExprPtr::operator[](ExprPtr other)
{
    return CuraFormulaeEngine::ast::make_expr_ptr<IndexExpr>(std::move(*this), std::move(other));
}
