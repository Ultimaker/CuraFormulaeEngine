#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <zeus/expected.hpp>

#include "cura-formulae-engine/eval.h"

namespace CuraFormulaeEngine::eval
{

enum class Error;

struct Value;

using Result = zeus::expected<Value, Error>;

} // namespace CuraFormulaeEngine::eval

namespace CuraFormulaeEngine::env
{

class Environment
{
public:
    virtual ~Environment() = default;

    [[nodiscard]] virtual std::optional<eval::Value> get(const std::string& key) const = 0;
    [[nodiscard]] virtual bool has(const std::string& key) const = 0;
    [[nodiscard]] virtual std::unordered_map<std::string, eval::Value> getAll() const = 0;
};

class EnvironmentMap : public Environment
{
private:
    std::unordered_map<std::string, eval::Value> environment_ = {};

public:
    EnvironmentMap() = default;
    explicit EnvironmentMap(const std::unordered_map<std::string, eval::Value>& map)
        : environment_(map)
    {
    }
    ~EnvironmentMap() override = default;

    [[nodiscard]] std::optional<eval::Value> get(const std::string& key) const noexcept override;

    [[nodiscard]] bool has(const std::string& key) const noexcept override;

    [[nodiscard]] std::unordered_map<std::string, eval::Value> getAll() const noexcept override;

    bool erase(const std::string& key) noexcept;

    void set(const std::string& key, const eval::Value& value) noexcept;

    void add(const std::unordered_map<std::string, eval::Value>& values);

    [[nodiscard]] EnvironmentMap clone() const noexcept;
};

class ChainableEnvironment : public Environment
{
private:
    const Environment* shadow_environment_{ nullptr };

public:
    explicit ChainableEnvironment(const Environment* shadow_environment = nullptr)
        : shadow_environment_(shadow_environment)
    {
    }
    ~ChainableEnvironment() override = default;

    [[nodiscard]] std::optional<eval::Value> get(const std::string& key) const noexcept override final;

    [[nodiscard]] bool has(const std::string& key) const noexcept override final;

    [[nodiscard]] std::unordered_map<std::string, eval::Value> getAll() const noexcept override final;

protected:
    [[nodiscard]] virtual std::optional<eval::Value> getImpl(const std::string& key) const noexcept = 0;

    [[nodiscard]] virtual bool hasImpl(const std::string& key) const noexcept = 0;

    [[nodiscard]] virtual std::unordered_map<std::string, eval::Value> getAllImpl() const noexcept = 0;
};

class LocalEnvironment : public ChainableEnvironment
{
private:
    EnvironmentMap local_environment_;

public:
    explicit LocalEnvironment(const Environment* shadow_environment = nullptr)
        : ChainableEnvironment(shadow_environment)
    {
    }
    ~LocalEnvironment() override = default;

    [[nodiscard]] std::optional<eval::Value> getImpl(const std::string& key) const noexcept override;

    [[nodiscard]] bool hasImpl(const std::string& key) const noexcept override;

    [[nodiscard]] std::unordered_map<std::string, eval::Value> getAllImpl() const noexcept override;

    void set(const std::string& key, const eval::Value& value);

    void add(const std::unordered_map<std::string, eval::Value>& values);
};

} // namespace CuraFormulaeEngine::env

namespace CuraFormulaeEngine::ast
{

struct Expr
{
    Expr() = default;
    Expr(const Expr&) = default;
    Expr(Expr&&) = default;
    Expr& operator=(const Expr&) = default;
    Expr& operator=(Expr&&) = default;
    virtual ~Expr() = default;

    /**
     * @brief Evaluates the expression in the given environment.
     *
     * @param environment The environment to evaluate the expression in.
     * @return eval_result The result of the evaluation.
     */
    [[nodiscard]] virtual eval::Result evaluate(const env::Environment* environment) const = 0;

    /**
     * @brief Returns the set of free variables in the expression.
     *
     * @return std::unordered_set<std::string> The set of free variables.
     */
    [[nodiscard]] virtual std::unordered_set<std::string> freeVariables() const = 0;

    /**
     * @brief Returns a string representation of the expression.
     *
     * @return std::string The string representation.
     */
    [[nodiscard]] virtual std::string toString() const = 0;

    /**
     * @brief Returns a deep copy of the expression.
     *
     * @return true if the expressions are equal, false otherwise.
     */
    [[nodiscard]] virtual bool deepEq(const Expr& other) const = 0;

    /**
     * @brief Traverses the expression tree and applies the visitor function to
     * each node.
     *
     * @param visitor The visitor function to apply to each node.
     */
    virtual void visitAll(std::function<void(const Expr&)> visitor) const = 0;
};

} // namespace CuraFormulaeEngine::ast
