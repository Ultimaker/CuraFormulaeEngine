#include "cura-formulae-engine/ast/ast.h"

namespace CuraFormulaeEngine::env {

std::optional<eval::Value>
EnvironmentMap::get(const std::string &key) const noexcept {
  auto iterator = environment_.find(key);
  if (iterator == environment_.end()) {
    return std::nullopt;
  }
  return iterator->second;
}

bool EnvironmentMap::has(const std::string &key) const noexcept {
  return environment_.find(key) != environment_.end();
}

std::unordered_map<std::string, eval::Value>
EnvironmentMap::getAll() const noexcept {
  return environment_;
}

bool EnvironmentMap::erase(const std::string &key) noexcept {
  return environment_.erase(key) > 0;
}

void EnvironmentMap::set(const std::string &key,
                         const eval::Value &value) noexcept {
  environment_.insert_or_assign(key, value);
}

void EnvironmentMap::add(
    const std::unordered_map<std::string, eval::Value> &values) {
  environment_.insert(values.begin(), values.end());
}

EnvironmentMap EnvironmentMap::clone() const noexcept {
  return EnvironmentMap{environment_};
}

std::optional<eval::Value>
LocalEnvironment::getImpl(const std::string &key) const noexcept {
  return local_environment_.get(key);
}

bool LocalEnvironment::hasImpl(const std::string &key) const noexcept {
  return local_environment_.has(key);
}

std::unordered_map<std::string, eval::Value>
LocalEnvironment::getAllImpl() const noexcept {
  return local_environment_.getAll();
}

void LocalEnvironment::set(const std::string &key, const eval::Value &value) {
  local_environment_.set(key, value);
}

void LocalEnvironment::add(
    const std::unordered_map<std::string, eval::Value> &values) {
  local_environment_.add(values);
}

std::optional<eval::Value>
ChainableEnvironment::get(const std::string &key) const noexcept {
  std::optional<eval::Value> value = getImpl(key);
  if (!value.has_value() && shadow_environment_) {
    value = shadow_environment_->get(key);
  }
  return value;
}

bool ChainableEnvironment::has(const std::string &key) const noexcept {
  return hasImpl(key) || (shadow_environment_ && shadow_environment_->has(key));
}

std::unordered_map<std::string, eval::Value>
ChainableEnvironment::getAll() const noexcept {
  std::unordered_map<std::string, eval::Value> all;
  if (shadow_environment_) {
    all = shadow_environment_->getAll();
  }

  auto scoped = getAllImpl();
  all.insert(scoped.begin(), scoped.end());
  return all;
}

} // namespace CuraFormulaeEngine::env
