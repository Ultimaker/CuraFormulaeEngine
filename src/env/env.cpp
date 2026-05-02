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
    env.set("int", eval::Value(eval::Value::rich_fn_t{ int_fn, int_function.getSignature() }));
    env.set("len", eval::Value(len));
    env.set("map", eval::Value(map));
    env.set("max", eval::Value(max));
    env.set("min", eval::Value(eval::Value::rich_fn_t{ min, min_function.getSignature() }));
    env.set("round", eval::Value(eval::Value::rich_fn_t{ round, round_function.getSignature() }));
    env.set("sum", eval::Value(sum));
    env.set("str", eval::Value(str));

    // Create math object with properties
    std::unordered_map<std::string, eval::Value> math_props;
    math_props["atan"] = eval::Value(math_atan);
    math_props["ceil"] = eval::Value(math_ceil);
    math_props["cos"] = eval::Value(math_cos);
    math_props["degrees"] = eval::Value(math_degrees);
    math_props["e"] = eval::Value(std::numbers::e);
    math_props["floor"] = eval::Value(math_floor);
    math_props["inf"] = eval::Value(std::numeric_limits<double>::infinity());
    math_props["log"] = eval::Value(eval::Value::rich_fn_t{ math_log, math_log_function.getSignature() });
    math_props["nan"] = eval::Value(std::nan("1"));
    math_props["pi"] = eval::Value(std::numbers::pi);
    math_props["sin"] = eval::Value(math_sin);
    math_props["tan"] = eval::Value(math_tan);
    math_props["tau"] = eval::Value(std::numbers::pi * 2.0);
    math_props["radians"] = eval::Value(math_radians);
    math_props["sqrt"] = eval::Value(math_sqrt);
    
    env.set("math", eval::Value(math_props));

    return env;
}();

} // namespace CuraFormulaeEngine::env
