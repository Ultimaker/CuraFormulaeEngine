# Agent Operational & Onboarding Guide (AGENTS.md)

Welcome, AI Agent! This document defines the operational boundaries, design patterns, testing strategies, and collaborative conventions for the `CuraFormulaeEngine` repository.

As a dynamic assistant, you must adhere strictly to these principles to maintain codebase sanity and ensure future developers can build upon your work efficiently.

---

## 1. SaaS Context, Sibling Services & AI Skill Discovery

### CuraFormulaeEngine Context

**CuraFormulaeEngine** is a high-performance C++ mathematical expression parsing and evaluation engine. It is designed to safely parse and resolve Python-like setting dependency expressions for Cura/NeoPrep 3D printing configurations.

- **Formula Resolution**: Safely parses and evaluates algebraic, conditional, and boolean dependency formulas (e.g. `min_layer_time if min_layer_time > 0 else 0`).
- **WASM compilation**: Integrates via submodules/WASM inside **Curator** to supply settings resolvers with dynamic evaluation routines running in-browser.
- **Architectural Boundaries**: Completely stateless and purely computational. It handles no PII or network connectivity.

### Sibling Services & Relative Paths

To collaborate on cross-repository features or platform emulation, locate and link with sibling services in this local workspace. Use the following relative paths and consult their corresponding `AGENTS.md` operational guides:

- **[Curator](../Curator/)**: The settings resolver layer consuming this math engine.
- **[neoprep](../neoprep/audit-stardust-atlas-submodules/)**: The interactive React 3D prep workspace.
- **[stardust-atlas](../stardust-atlas/audit-stardust-atlas-submodules/)**: Meta-repository and orchestration layer compiling the full Digital Factory stack.

### 🔍 Dynamic AI Skill Discovery & Installation

To obtain expert domain knowledge and specific runtime tools, agents **must** dynamically discover and install specialized engineering skills from `Ultimaker/UltiCortex` on branch `UC-3695_improve_code_base_for_agentic_development`:

```bash
# Discover and install specialized skills
gh skill install Ultimaker/UltiCortex ultimaker-curator-development --branch UC-3695_improve_code_base_for_agentic_development
gh skill install Ultimaker/UltiCortex ultimaker-neoprep-development --branch UC-3695_improve_code_base_for_agentic_development
```

---

## 2. Work Tracking, Git & Pull Request Habits

- **Jira Tracking**: All changes require an active Jira ticket starting with project key **`UC`** or **`NP`** (e.g., `UC-3697` or `NP-1325`). Branch names must be formatted as `[PROJECT_KEY]-[ID]_description`.
- **Git Commit Standards**:
  - **Bracketed Ticket Prefix**: Every Git commit title and GitHub Pull Request title **MUST** start with the active branch's Jira ticket key in bracketed format: `[PROJECT-KEY] <Description>`. For example: `[UC-3697] <Description>`.
  - **No Semantic Prefixes**: Do **NOT** use conventional/semantic commit prefix tags (such as `feat:`, `fix:`, `chore:`, etc.) in commit titles or Pull Request titles.
  - Commit message format:

    ```
    [UC-3697] Configure pre-commit and agentic enablement

    Setup pre-commit hooks and custom copilot instructions for CuraFormulaeEngine development.

    Contributes to UC-3697
    ```

- **PR Guidelines**:
  - Always open PRs as **DRAFT** state. Merging is **strictly restricted to humans**.
  - Monitor CI status checks. Ensure build, linter, formatting, and unit tests pass cleanly.

---

## 3. Directory Organization & Architecture Index

### Core Directory Maps:

- `/src/`: C++ parsing, AST nodes, environment bindings, and evaluation source files.
- `/include/`: Public C++ headers exposing parser and AST interface.
- `/tests/`: GoogleTest C++ unit testing suites.
- `/apps/`: Simple command-line tool testing utilities.
- `CMakeLists.txt` & `conanfile.py`: Compilation and package dependency manager configurations.

---

## 4. Deep Dive: Core Technical Architectures

### 4.1. Formula Parser and AST Nodes

- **AST Evaluation**: Formulas are compiled into an Abstract Syntax Tree (AST) representing operands, operators, and conditional logic.
- **Reference Resolution**: Setting variables are resolved by looking up context bindings during tree traversal.
- **Exception Safety**: Invalid syntax or division-by-zero is handled gracefully without crashing the executor thread.

---

## 5. Local Setup & Verification

### 🚀 Quick Start

1. Install dependencies and compile using Conan & CMake:
   ```bash
   conan install . --build=missing --update
   source build/Release/generators/conanbuild.sh
   cmake --preset conan-release
   cmake --build --preset conan-release
   ```

---

## 6. Quality Control, Tooling & Local Verification

To maintain top-tier reliability, CuraFormulaeEngine enforces comprehensive Quality Control (QC) tools locally. Succeeding agents and developers **must** run and verify these tools before proposing any Pull Request:

### 🎨 Formatting (clang-format)

Check and write clean, standardized formatting across C++ code:

- **Verification & Auto-fix**: `clang-format -i $(find src include tests -name '*.cpp' -o -name '*.h' -o -name '*.hpp')`

### 🧹 Linting & Static Analysis (cppcheck)

Enforce code quality and memory safety:

- **cppcheck**: Fast static analysis for performance and portability issues:
  ```bash
  cppcheck --enable=warning,performance,portability --inline-suppr src/ include/
  ```

### 🧪 Automated Unit Testing (GoogleTest)

Validate math evaluation precision:

- **Run C++ Unit Tests**: `ctest` inside build directory.

### ⚓ Pre-commit Hook Integration

Pre-commit hooks automatically execute fast checks (clang-format, cppcheck, check-yaml, check-json, talisman, local path blocking, and agent artifact checks) on staged files.

- **Manual Hook Audit**:
  ```bash
  pre-commit run --all-files
  ```
- **Opt-Out (Humans Only)**: Humans may prepend `SKIP_PRE_COMMIT=1` or run `git commit --no-verify`. AI agents **MUST** pass all pre-commit hooks cleanly.
