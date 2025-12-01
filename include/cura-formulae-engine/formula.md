# Formula

Cura setting formula's allow users to express setting values as an expression. This not only allows users to use
mathematical expressions, but also allows users to use other settings as input variables and functions. These
expressions are a subset of the Python language. In order to properly interpreted cura setting stacks being able
to parse and intepret these formula(-string)s is required.

Interpreting these formula's is done in two steps:

1. Parsing the formula into an abstract syntax tree (AST)
2. Evaluating the AST

To assist with the evaluation three data structures are used:

- The AST (abstract syntax tree)
- Eval Value
- Environment

The sections below will explain the various steps and data structures in more detail.

### Parser

Parsing is the process of converting a string into a data structure, in this case
an [abstract syntax tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree), see [next section](#abstract-syntax-tree)
for more information on the AST used. The parser itself is implemented using [lexy](https://lexy.foonathan.net/). Lexy
is a compile time parser combinator library. It allows for the creation of a parser by combining small parsers into a
larger parser. The combining of parser-production is done at compile time.

Unlike other parsers lexy doesn't typically backtrack or look ahead more than a few token, it is possible but
discouraged. This has some benefits, and some downsides. The benefit is that the parser is very fast; you are guaranteed
that the parser will run in $O(n)$ time. The downside is that the parser doesn't always partition well into modulair
parsing parts. For instance, function application and array indexing. The grammar for these two productions are very
similar.

```
FN APPLICATION : { variable } '(' { expression } ')'
ARRAY INDEXING : { variable } '[' { expression } ']'
```

As both productions start with the same production we have to combine these productions in our parser combinator. If we
were to split them then the variable would be parsed (and rejected) by the first parser. As we don't backtrack it is not
possible anymore to parse the other production.

For ease of use a wrapper function around the `lexy::parse` call is provided, this wrapper function is located at
`CuraFormulaeEngine::parser::parse`. The input to this function is a single argument, a `string_view` to the parse input string.
This function will return a `result<ast::ExprPtr, error_t>` type. If the parsing was successful the result will contain
the AST. If the parsing was not successful the result will contain an error message.

### Abstract Syntax Tree

An abstract syntax tree (AST) is a representation of the formula expression. Each AST node can be considered a building
block of the expression. For all primary types (int, double, string, bool) and all operators (addition, subtraction,
multiplication, etc.) a corresponding AST node is defined. Additionally, AST nodes are defined for python specific
expressions such as list comprehensions, list literals, etc. These AST nodes can then be combined to form the AST for
the entire expression.

All AST nodes inherit from the `CuraFormulaeEngine::ast::Expr` class. This class provides the following methods

- **free_variables**: finds all free variables in the AST. free variables are the variables from an expression that
  don't get an assignment from the expression itself. For instance, in the expression `[x for x in list]` the variable
  `x` is not free since it gets assigned by the expression itself while the variable `list` is free.
- **evaluate**: evaluates (resolves) all AST nodes into an eval value. See [evaluation](#evaluation) for more
  information.

### Eval Value

Eval value is the data structure used to represent python values. It is a variant type that can hold any of the
following types:

- `bool`
- `long`
- `double`
- `std::string`
- `std::vector<EvalValue>`
- `std::function<EvalResult(const std::vector<EvalValue>&)>`

Pythons dynamic typing is implemented through the eval value type. Functions are added for all operations that can be
performed on eval values. Similar to the AST overloads for all basic operators are added. However, instead of building a
new AST node the operation is directly performed on the eval value. E.g. `eval_value + eval_value` will return a new
eval value that is the result of the addition of the two eval values.

Some operations might not be possible on certain types. In python this would be a run time error. To reflect this an
`eval_result = result<eval_value, error>` type is introduced. When performing the eval function for an erroneous
(for example array out of bounds) expression the error variant type is returned. If the operation was successful the
result will contain the resulting eval value.

### Environment

The environment is a data structure that holds all the variables that are available to the formula. It is a map of
variable names to eval values. The environment is used during evaluation of the AST to resolve all free variables.

Additionally, a global environment is defined. Contained in the global environment are some variable/functions from the
standard library (think of `math.sin`, `math.pi`, `map` ect).

### Evaluation

During evaluation of the AST, the AST is resolved. Starting from the leaf nodes, each node is converted into an eval
value. In order to properly evaluate the AST a local environment is required. In the local environment all free
variables are resolved to their corresponding values.

### Example

The following example shows the evaluation of the expression `x ** 2` with the variable `x` set to `2`.

```cpp
#include "formula/ast/expr_ptr.h"
#include "formula/env/env.h"
#include "formula/eval.h"
#include "formula/parser/parser.h"

#include <spdlog/spdlog.h>

#include <string_view>
#include <iostream>

using namespace std::string_view_literals;

int main(int argc, const char** argv)
{
    auto input = "x ** 2"sv;
    auto message = CuraFormulaeEngine::parser::parse(input);

    CuraFormulaeEngine::env::LocalEnvironment custom_env { &CuraFormulaeEngine::env::std_env };
    custom_env.set("x", CuraFormulaeEngine::eval::Value { 2L });
    
    if (!message.has_value())
    {
        spdlog::warn("Parse error");
        return 1;
    }

    const auto& expr = message.value();
    const auto eval_result = expr.evaluate(custom_env);
    
    if (!eval_result.has_value())
    {
        const auto error = eval_result.error();
        spdlog::warn("Expr {} results in eval error", Expr.toString());
        return 1;
    }

    const auto& eval = eval_result.value()
    spdlog::info("{} = {}", Expr.toString(), eval.toString());

    return 0;
}
```
