---
name: build-test-and-deployment
description: Build, test, and deployment verification commands detected for this repository, with Freshness Before Evidence rules.
trigger: always_on
---
# Build, Test & Deployment Verification

1. **Build Commands**:
   - `conan install . --build=missing --update`
   - `cmake --preset conan-release`
   - `cmake --build --preset conan-release`
   - Builds must complete cleanly (zero warnings) before PR creation.
2. **Package Registry Authentication**:
   - Obtain `GITHUB_TOKEN` (scope `read:packages`) for private `@ultimaker` packages via the keyring-first chain in `scripts/get_github_token.sh` — source it (`. scripts/get_github_token.sh`) instead of hand-exporting: (a) an existing env var wins, (b) else the system keyring (Linux: `secret-tool lookup service github user "$USER"`), (c) else a gitignored `.env`/`.env.local` fallback with a warning. Store it once per machine, user-specific — never a hardcoded username: `echo -n "<PAT>" | secret-tool store --label="$USER-github-token" service github user "$USER"`. Never write tokens to disk, logs, or git.
3. **Test Commands**:
   - `ctest --preset conan-release --output-on-failure`
4. **Artifact Isolation**:
   - Keep generated build outputs, intermediate binaries, and logs out of git (`build/` is gitignored).
5. **Freshness Before Evidence**:
   - Rebuild binaries (`cmake --build --preset conan-release`) before treating a test observation as evidence. An observation is only valid if the artifacts observed were compiled from the current source state. An unverified stale build is a false observation.

## Semantic Release & Version Tagging

1. **SemVer Version Tagging**:
   - Releases MUST follow Semantic Versioning (`MAJOR.MINOR.PATCH`).
   - Tags MUST be created on main branch commits after PR integration.
2. **Automated Changelog Generation**:
   - Changelogs are generated directly from Pull Request titles. Ensure PR titles follow the `[UC-123] <Descriptive Title>` or `[NP-123] <Descriptive Title>` standard.
3. **Version Synchronization**:
   - Single source of truth for versioning is `conandata.yml` (`version: "..."`).
   - `conanfile.py` reads `conandata.yml` and injects `CURA_FORMULA_VERSION_VERSION` into CMake. A release must update `conandata.yml`.
