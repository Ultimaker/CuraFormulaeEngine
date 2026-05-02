#pragma once

#include "cura-formulae-engine/eval.h"

#include <string>
#include <vector>

namespace CuraFormulaeEngine::env
{

struct RoundFunction
{
	[[nodiscard]] eval::Result operator()(const std::vector<eval::Value>& args) const noexcept;
	[[nodiscard]] std::vector<std::string> getSignature() const noexcept;
};

extern const RoundFunction round_function;
extern const eval::Value::fn_t round;

} // namespace CuraFormulaeEngine::env
