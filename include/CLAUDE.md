# include/ — Subsystem Instructions

<!-- Loaded on demand: Claude Code reads this file when it opens code in this
     directory, so it costs nothing during work elsewhere in the repository.
     Keep it to what is true HERE and different from the root AGENTS.md. -->

**Scope:** 70 files, predominantly .h, .md.

## What this subsystem does

`include/` defines the public C++ API and Abstract Syntax Tree (AST) interface for `cura-formulae-engine`. It exposes the Lexy-based expression parser (`parser/`), symbol table environment bindings (`env/`), AST node hierarchies (`ast/`), and value/evaluation types (`eval.h`, `cura-formulae-engine.h`). This surface is consumed directly by **Curator** (the C++ settings resolver) and compiled into WebAssembly via Emscripten for interactive browser-based formula evaluation in **NeoPrep**.

## Conventions specific to this directory

- **Cross-Repository Consumer Contract**: Every exported symbol, method signature, and enum in `include/` is a shared contract across repository boundaries. Additive changes only; do not remove or alter existing signatures without corresponding updates in `Curator`.
- **Pure Computation & Determinism**: Headers in this subsystem must remain strictly stateless and free of side-effects or network dependencies.
- **Error Value Propagation**: Functions return `zeus::expected<T, Error>` (aliased as `Result`) rather than throwing exceptions across the public boundary.
- **Emscripten Bindings**: Under `#ifdef EMSCRIPTEN`, headers provide `emscripten::val` conversions (`toEmscripten()`). Ensure any new variant type in `eval::Value` is properly mirrored to JavaScript values.

## Commands

- **Build Native**: `cmake --preset conan-release && cmake --build --preset conan-release` (from repository root).
- **Run Tests**: `ctest --test-dir build/Release --output-on-failure` (or `ctest --preset conan-release`).

## Gotchas

- **Header-Only Invariants in Grammar**: `include/cura-formulae-engine/parser/expr_grammar.h` contains compile-time Lexy PEG parser rules. Modifying grammar productions directly affects operator precedence and associativity across all dependent AST node parsers.
- **AST Node Pointer Semantics**: AST nodes use `ast::ExprPtr` (a smart-pointer wrapped expression) for tree assembly. When constructing expressions in tests or AST transforms, use `ast::make_expr_ptr<T>(...)`.

---

Repository-wide orientation is in the root `AGENTS.md`; normative rules are in
`.agents/rules/`. Do not restate either here.
