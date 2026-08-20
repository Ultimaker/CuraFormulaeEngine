---
name: jira-commit-standards
description: Jira work tracking and commit message standards.
trigger: always_on
---
# Jira & Git Commit Standards

1. **Jira Work Tracking**:
   - All branches MUST reference an active Jira ticket starting with project key `UC` or `NP` (e.g. `UC-3697-short-description` or `NP-1325_fix_eval`).
2. **Commit Title Standard**:
   - Every commit title MUST start with bracketed Jira ticket key: `[UC-123] <Descriptive Title>` or `[NP-123] <Descriptive Title>`.
   - Do NOT use semantic commit prefixes (`feat:`, `fix:`, `chore:`, `refactor:`) in commit or PR titles.
3. **Commit Body Requirement**:
   - Non-trivial commits MUST include an explanatory commit body detailing why the change was made, architecture trade-offs, and reference the ticket with `Contributes to <KEY>`.
   - Example:
     ```text
     [UC-3697] Configure pre-commit and agentic enablement

     Setup pre-commit hooks and custom copilot instructions for CuraFormulaeEngine development.

     Contributes to UC-3697
     ```
4. **Pull Request Policy**:
   - Always open PRs in **DRAFT** state.
   - Merging is strictly restricted to human developers.
