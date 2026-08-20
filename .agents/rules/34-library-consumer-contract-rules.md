---
name: library-consumer-contract
description: This repository's public surface is consumed by other repositories (Curator, NeoPrep) as a Conan 2 package and WebAssembly module — every exported symbol and binding is a cross-repository contract.
trigger: glob
glob: "include/**,src/eval.cpp,src/cura-formulae-engine/**"
paths:
  - "include/**"
  - "src/eval.cpp"
  - "src/cura-formulae-engine/**"
---
# Library Consumer Contract

This repository is distributed as a Conan 2 package (`cura-formulae-engine/<version>@ultimaker/testing`) and consumed by other repositories rather than run solely as an end-user application.

## Evidence & Architecture

- **Public Header Surface (`include/cura-formulae-engine/`)**: Exposes AST nodes, parser combinators, environment symbol tables, and value evaluation variants.
- **Direct Downstream Consumer**: **Curator** (`../Curator/conanfile.py`) requires `cura-formulae-engine/<version>@ultimaker/testing` for setting dependency graph evaluation.
- **WASM Consumer**: Curator compiles `CuraFormulaeEngine` into WebAssembly (`@ultimaker/curatorjs`), consumed by **NeoPrep** for client-side evaluation.
- **Standalone Verification**: An interactive REPL CLI is provided in `apps/cmdline_parser/` for local formula parsing and evaluation debugging.

## What Constitutes a Breaking Change

Within `include/cura-formulae-engine/` and `src/eval.cpp`, all of the following are contract changes, not isolated internal refactors:

1. **Renaming or removing any exported symbol** — `ast::ExprPtr`, `parser::parse()`, `eval::Value`, `eval::Result`, `eval::Error`, `env::Environment`.
2. **Modifying the `eval::Value` variant or `eval::Error` enum values**: downstream consumers switch exhaustively on these variants.
3. **Changing Emscripten bindings or conversions (`Value::toEmscripten()`)**: breaks JavaScript/TypeScript interop in `@ultimaker/curatorjs` and NeoPrep.
4. **Changing formula evaluation semantics or operator precedence**: alters setting resolution results across the 3D slicing pipeline.

## Consumer Coordination Protocol

1. **Check Consumers**: Search downstream repositories for requirements:
   ```bash
   gh search code --owner Ultimaker --filename conanfile.py "cura-formulae-engine"
   ```
2. **Additive Evolution**: Where a breaking shape is unavoidable, provide backwards-compatible overloads or aliases.
3. **Package Versioning**: Update `version` in `conandata.yml` following Semantic Versioning rules, and update `conanfile.py` in downstream consumers (`Curator`).
4. **Explanatory Commit Message**: Clearly describe any changes to the public API and AST/evaluation behaviour so downstream maintainers understand the upgrade path.
