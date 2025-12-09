#include "cura-formulae-engine/env/env.h"

#include "cura-formulae-engine/ast/ast.h"
#include "cura-formulae-engine/env/abs.h"
#include "cura-formulae-engine/env/all.h"
#include "cura-formulae-engine/env/any.h"
#include "cura-formulae-engine/env/float_fn.h"
#include "cura-formulae-engine/env/int_fn.h"
#include "cura-formulae-engine/env/len.h"
#include "cura-formulae-engine/env/map.h"
#include "cura-formulae-engine/env/math_atan.h"
#include "cura-formulae-engine/env/math_ceil.h"
#include "cura-formulae-engine/env/math_cos.h"
#include "cura-formulae-engine/env/math_degrees.h"
#include "cura-formulae-engine/env/math_floor.h"
#include "cura-formulae-engine/env/math_log.h"
#include "cura-formulae-engine/env/math_radians.h"
#include "cura-formulae-engine/env/math_sin.h"
#include "cura-formulae-engine/env/math_sqrt.h"
#include "cura-formulae-engine/env/math_tan.h"
#include "cura-formulae-engine/env/max.h"
#include "cura-formulae-engine/env/min.h"
#include "cura-formulae-engine/env/round.h"
#include "cura-formulae-engine/env/str.h"
#include "cura-formulae-engine/env/sum.h"
#include "cura-formulae-engine/eval.h"

#include <cmath>
#include <limits>
#include <numbers>

namespace CuraFormulaeEngine::env
{

/**
 * @brief The standard environment.
 */
const EnvironmentMap std_env = []()
{
    EnvironmentMap env;

    env.set("abs", eval::Value(abs));
    env.set("all", eval::Value(all));
    env.set("any", eval::Value(any));
    env.set("float", eval::Value(float_fn));
    env.set("int", eval::Value(int_fn));
    env.set("len", eval::Value(len));
    env.set("map", eval::Value(map));
    env.set("math.atan", eval::Value(math_atan));
    env.set("math.ceil", eval::Value(math_ceil));
    env.set("math.cos", eval::Value(math_cos));
    env.set("math.degrees", eval::Value(math_degrees));
    env.set("math.e", eval::Value(std::numbers::e));
    env.set("math.floor", eval::Value(math_floor));
    env.set("math.inf", eval::Value(std::numeric_limits<double>::infinity()));
    env.set("math.log", eval::Value(math_log));
    env.set("math.nan", eval::Value(std::nan("1")));
    env.set("math.pi", eval::Value(std::numbers::pi));
    env.set("math.sin", eval::Value(math_sin));
    env.set("math.tan", eval::Value(math_tan));
    env.set("math.tau", eval::Value(std::numbers::pi * 2.0));
    env.set("math.radians", eval::Value(math_radians));
    env.set("math.sqrt", eval::Value(math_sqrt));
    env.set("max", eval::Value(max));
    env.set("min", eval::Value(min));
    env.set("round", eval::Value(round));
    env.set("sum", eval::Value(sum));
    env.set("str", eval::Value(str));

    return env;
}();

} // namespace CuraFormulaeEngine::env
