#include "cura-formulae-engine/ast/ast.h"

namespace CuraFormulaeEngine::env
{

std::optional<eval::Value> EnvironmentMap::get(const std::string& key) const noexcept
{
    if (!has(key))
    {
        return std::nullopt;
    }
    return environment_.at(key);
}

bool EnvironmentMap::has(const std::string& key) const noexcept
{
    return environment_.find(key) != environment_.end();
}

std::unordered_map<std::string, eval::Value> EnvironmentMap::getAll() const noexcept
{
    return environment_;
}

bool EnvironmentMap::erase(const std::string& key) noexcept
{
    return environment_.erase(key) > 0;
}

void EnvironmentMap::set(const std::string& key, const eval::Value& value) noexcept
{
    environment_.insert_or_assign(key, value);
}

EnvironmentMap EnvironmentMap::clone() const noexcept
{
    return EnvironmentMap{environment_};
}

std::optional<eval::Value> LocalEnvironment::get(const std::string& key) const noexcept
{
    if (local_environment_.has(key))
    {
        return local_environment_.get(key);
    }
    if (shadow_environment_ && shadow_environment_->has(key))
    {
        return shadow_environment_->get(key);
    }
    return std::nullopt;
}

bool LocalEnvironment::has(const std::string& key) const noexcept
{
    return local_environment_.has(key) || (shadow_environment_ && shadow_environment_->has(key));
}

std::unordered_map<std::string, eval::Value> LocalEnvironment::getAll() const noexcept
{
    std::unordered_map<std::string, eval::Value> all;
    if (shadow_environment_) {
        all = shadow_environment_->getAll();
    }

    auto scoped = local_environment_.getAll();
    all.insert(scoped.begin(), scoped.end());
    return all;
}

void LocalEnvironment::set(const std::string& key, const eval::Value& value)
{
    local_environment_.set(key, value);
}

} // namespace CuraFormulaeEngine::env