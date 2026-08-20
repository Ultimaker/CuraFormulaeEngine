---
name: cura-formulae-engine-conventions
description: Repository-specific architecture conventions, Catch2 test organization, snake_case file naming, AST pointer patterns, and zeus::expected error handling.
trigger: glob
glob: "src/**,include/**,tests/**,apps/**"
paths:
  - "src/**"
  - "include/**"
  - "tests/**"
  - "apps/**"
---
# CuraFormulaeEngine Conventions

1. **Test Organization & Applications**:
   - All unit tests belong in the top-level `tests/` directory and use Catch2 (`Catch2::Catch2WithMain`).
   - Tests are discovered via `catch_discover_tests` in `tests/CMakeLists.txt` and executed via `ctest --preset conan-release --output-on-failure`.
   - Standalone tools reside in `apps/` (e.g. `apps/cmdline_parser/cmdline_parser.cpp` provides an interactive CLI REPL for expression testing).
   - New AST nodes, parser rules, and evaluation primitives MUST include corresponding test cases verifying both valid syntax/evaluation and expected error conditions.

2. **File Naming & Namespace Conventions**:
   - Source files, header files, and test files MUST follow `snake_case` naming (`snake_case.cpp`, `snake_case.h`).
   - Namespace hierarchy:
     - `CuraFormulaeEngine`: Top-level namespace
     - `CuraFormulaeEngine::ast`: Abstract syntax tree node structures and pointer aliases (`ExprPtr`, constructed via `ast::make_expr_ptr<T>(...)`)
     - `CuraFormulaeEngine::parser`: Lexy combinator grammar rules and `parse()` entry point
     - `CuraFormulaeEngine::env`: Evaluation environment, symbol table bindings, and builtin functions
     - `CuraFormulaeEngine::eval`: Value variant, arithmetic/boolean operator overloads, and evaluation result types

3. **Symbol Table & Builtin Hierarchy**:
   - Top-level builtins (`std_env`): `abs`, `all`, `any`, `float`, `int`, `len`, `map`, `max`, `min`, `round`, `str`, `sum`.
   - Math namespace object (`math.*`): `sin`, `cos`, `tan`, `atan`, `sqrt`, `floor`, `ceil`, `log`, `degrees`, `radians`, `pi`, `e`, `tau`, `inf`, `nan` are registered as properties on the `math` object.

4. **Error Handling with `zeus::expected`**:
   - Parsing and evaluation functions return `zeus::expected<T, Error>` (`eval::Result` alias) to maintain strict, deterministic error handling without runtime aborts.
   - Evaluation errors are categorized via `eval::Error` enum (`TypeMismatch`, `UndefinedVariable`, `DivisionByZero`, `InvalidNumberOfArguments`, `IndexOutOfBounds`, `ValueError`).
