#pragma once

#include "cura-formulae-engine/eval.h"

#include <string>
#include <vector>

namespace CuraFormulaeEngine::env
{

struct MathLogFunction
{
	[[nodiscard]] eval::Result operator()(const std::vector<eval::Value>& args) const noexcept;
	[[nodiscard]] std::vector<std::string> getSignature() const noexcept;
};

extern const MathLogFunction math_log_function;
extern const eval::Value::fn_t math_log;

} // namespace CuraFormulaeEngine::env
