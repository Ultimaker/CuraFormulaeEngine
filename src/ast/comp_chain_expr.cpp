#include "cura-formulae-engine/ast/comp_chain_expr.h"
#include "cura-formulae-engine/eval.h"

#include <string>
#include <unordered_set>
#include <variant>
#include <vector>

namespace CuraFormulaeEngine::ast
{

[[nodiscard]] std::string ComparisonChainExpr::toString() const noexcept
{
    std::string result;
    for (size_t i = 0; i < operators.size(); ++i)
    {
        result += expressions[i].toString();
        switch (operators[i])
        {
        case Equals:
            result += " == ";
            break;
        case NotEquals:
            result += " != ";
            break;
        case LessThan:
            result += " < ";
            break;
        case GreaterThan:
            result += " > ";
            break;
        case LessThenEqual:
            result += " <= ";
            break;
        case GreaterThenEqual:
            result += " >= ";
            break;
        case NotMember:
            result += " not in ";
            break;
        case Member:
            result += " in ";
            break;
        }
    }
    result += expressions.back().toString();
    return result;
}

[[nodiscard]] eval::Result ComparisonChainExpr::evaluate(const env::Environment* environment) const noexcept
{
    assert(expressions.size() == operators.size() + 1);

    const auto left_value_result = expressions[0].evaluate(environment);
    if (!left_value_result.has_value())
    {
        return zeus::unexpected(left_value_result.error());
    }
    auto left_value = left_value_result.value();

    for (size_t i = 0; i < operators.size(); i ++)
    {
        const auto right_value_result = expressions[i + 1].evaluate(environment);
        if (!right_value_result.has_value())
        {
            return zeus::unexpected(right_value_result.error());
        }
       const auto& right_value = right_value_result.value();

        eval::Result comparison_result;
        switch (operators[i])
        {
        case Equals:
            comparison_result = left_value == right_value;
            break;
        case NotEquals:
            comparison_result = !(left_value == right_value);
            break;
        case LessThan:
            comparison_result = left_value < right_value;
            break;
        case GreaterThan:
            comparison_result = left_value > right_value;
            break;
        case LessThenEqual:
            comparison_result = left_value_result.value() <= right_value_result.value();
            break;
        case GreaterThenEqual:
            comparison_result = left_value_result.value() >= right_value_result.value();
            break;
        case Member:
        case NotMember:
            if (!right_value_result.has_value())
            {
                return zeus::unexpected(right_value_result.error());
            }

            const auto& rhs_value = right_value_result.value();
            if (!std::holds_alternative<std::vector<eval::Value>>(rhs_value.value))
            {
                return zeus::unexpected(eval::Error::TypeMismatch);
            }

            comparison_result = operators[i] == NotMember;

            const auto& list = std::get<std::vector<eval::Value>>(rhs_value.value);
            for (const auto& item : list)
            {
                if (left_value == item)
                {
                    comparison_result = operators[i] == Member;
                    break;
                }
            }
            break;
        }

        if (!comparison_result.has_value())
        {
            return zeus::unexpected(comparison_result.error());
        }

        if (!comparison_result.value().isTruthy())
        {
            return false;
        }

        left_value = right_value;
    }

    return true;
}

[[nodiscard]] std::unordered_set<std::string> ComparisonChainExpr::freeVariables() const noexcept
{
    std::unordered_set<std::string> free_vars;
    for (const auto& expr : expressions)
    {
        const auto expr_free_vars = expr.freeVariables();
        free_vars.insert(expr_free_vars.begin(), expr_free_vars.end());
    }
    return free_vars;
}

[[nodiscard]] bool ComparisonChainExpr::deepEq(const Expr& other) const noexcept
{
    if (const auto other_comp_chain_expr = dynamic_cast<const ComparisonChainExpr*>(&other))
    {
        if (operators.size() != other_comp_chain_expr->operators.size() || expressions.size() != other_comp_chain_expr->expressions.size())
        {
            return false;
        }
        for (size_t i = 0; i < operators.size(); ++i)
        {
            if (operators[i] != other_comp_chain_expr->operators[i])
            {
                return false;
            }
        }
        for (size_t i = 0; i < expressions.size(); ++i)
        {
            if (!expressions[i].deepEq(other_comp_chain_expr->expressions[i]))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

void ComparisonChainExpr::visitAll(std::function<void(const Expr&)> visitor) const noexcept
{
    visitor(*this);

    for (const auto& expr : expressions)
    {
        expr.visitAll(visitor);
    }
}

} // namespace CuraFormulaeEngine::ast

CuraFormulaeEngine::ast::ExprPtr operator==(CuraFormulaeEngine::ast::ExprPtr lhs, CuraFormulaeEngine::ast::ExprPtr rhs)
{
    std::vector<CuraFormulaeEngine::ast::ExprPtr> expressions;
    expressions.push_back(std::move(lhs));
    expressions.push_back(std::move(rhs));
    return CuraFormulaeEngine::ast::make_expr_ptr<CuraFormulaeEngine::ast::ComparisonChainExpr>(
        std::move(expressions),
        std::vector{ CuraFormulaeEngine::ast::Equals }
    );
}

CuraFormulaeEngine::ast::ExprPtr operator!=(CuraFormulaeEngine::ast::ExprPtr lhs, CuraFormulaeEngine::ast::ExprPtr rhs)
{
    std::vector<CuraFormulaeEngine::ast::ExprPtr> expressions;
    expressions.push_back(std::move(lhs));
    expressions.push_back(std::move(rhs));
    return CuraFormulaeEngine::ast::make_expr_ptr<CuraFormulaeEngine::ast::ComparisonChainExpr>(
        std::move(expressions),
        std::vector{ CuraFormulaeEngine::ast::NotEquals }
    );
}

CuraFormulaeEngine::ast::ExprPtr operator>=(CuraFormulaeEngine::ast::ExprPtr lhs, CuraFormulaeEngine::ast::ExprPtr rhs)
{
    std::vector<CuraFormulaeEngine::ast::ExprPtr> expressions;
    expressions.push_back(std::move(lhs));
    expressions.push_back(std::move(rhs));
    return CuraFormulaeEngine::ast::make_expr_ptr<CuraFormulaeEngine::ast::ComparisonChainExpr>(
        std::move(expressions),
        std::vector{ CuraFormulaeEngine::ast::GreaterThenEqual }
    );
}

CuraFormulaeEngine::ast::ExprPtr operator>(CuraFormulaeEngine::ast::ExprPtr lhs, CuraFormulaeEngine::ast::ExprPtr rhs)
{
    std::vector<CuraFormulaeEngine::ast::ExprPtr> expressions;
    expressions.push_back(std::move(lhs));
    expressions.push_back(std::move(rhs));
    return CuraFormulaeEngine::ast::make_expr_ptr<CuraFormulaeEngine::ast::ComparisonChainExpr>(
        std::move(expressions),
        std::vector{ CuraFormulaeEngine::ast::GreaterThan }
    );
}

CuraFormulaeEngine::ast::ExprPtr operator<=(CuraFormulaeEngine::ast::ExprPtr lhs, CuraFormulaeEngine::ast::ExprPtr rhs)
{
    std::vector<CuraFormulaeEngine::ast::ExprPtr> expressions;
    expressions.push_back(std::move(lhs));
    expressions.push_back(std::move(rhs));
    return CuraFormulaeEngine::ast::make_expr_ptr<CuraFormulaeEngine::ast::ComparisonChainExpr>(
        std::move(expressions),
        std::vector{ CuraFormulaeEngine::ast::LessThenEqual }
    );
}

CuraFormulaeEngine::ast::ExprPtr operator<(CuraFormulaeEngine::ast::ExprPtr lhs, CuraFormulaeEngine::ast::ExprPtr rhs)
{
    std::vector<CuraFormulaeEngine::ast::ExprPtr> expressions;
    expressions.push_back(std::move(lhs));
    expressions.push_back(std::move(rhs));
    return CuraFormulaeEngine::ast::make_expr_ptr<CuraFormulaeEngine::ast::ComparisonChainExpr>(
        std::move(expressions),
        std::vector{ CuraFormulaeEngine::ast::LessThan }
    );
}
