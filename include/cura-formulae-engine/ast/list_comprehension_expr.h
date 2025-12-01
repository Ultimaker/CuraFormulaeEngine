#pragma once

#include "cura-formulae-engine/ast/ast.h"
#include "expr_ptr.h"
#include "cura-formulae-engine/eval.h"

#include <optional>
#include <unordered_set>
#include <variant>
#include <vector>
#include <zeus/expected.hpp>

namespace CuraFormulaeEngine::ast
{

struct ListComprehensionExpr final : Expr
{
    struct loop
    {
        ExprPtr iterator_key;
        ExprPtr iterable;
        std::vector<ExprPtr> conditions;

        loop(ExprPtr iterator_key, ExprPtr iterable, std::vector<ExprPtr> conditions)
            : iterator_key(std::move(iterator_key))
            , iterable(std::move(iterable))
            , conditions(std::move(conditions))
        {
        }
    };

    ExprPtr iterator;
    std::vector<loop> loops;

    ListComprehensionExpr(ExprPtr iterator, std::vector<loop> loops)
        : iterator(std::move(iterator))
        , loops(std::move(loops))
    {
    }

    [[nodiscard]] std::string toString() const noexcept final;

    std::optional<eval::Error> handle_loop(const size_t loop_index, env::LocalEnvironment& local_environment, std::vector<eval::Value>& results) const;

    [[nodiscard]] eval::Result evaluate(const env::Environment* environment) const noexcept final;

    [[nodiscard]] std::unordered_set<std::string> freeVariables() const noexcept final;

    [[nodiscard]] bool deepEq(const Expr& other) const noexcept final;

    void visitAll(std::function<void(const Expr&)> visitor) const noexcept final;

};

} // namespace CuraFormulaeEngine::ast
