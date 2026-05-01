# Windows-Only Closure Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Remove Linux-facing build/test/documentation entry points so the repository presents and runs as Windows-only by default.

**Architecture:** Keep the existing Windows build and GUI smoke flow intact while disconnecting the top-level CMake graph from `test/`, collapsing CI onto `windows-latest`, and rewriting the README so it no longer promises Linux support. Leave historical files under `test/` in place, but make them unreachable from the default build path.

**Tech Stack:** CMake, GitHub Actions, PowerShell, Markdown

---

### Task 1: Remove Linux Default Build Entry Points

**Files:**
- Modify: `D:\Dev\Code\surakarta_traditional\CMakeLists.txt`

- [ ] **Step 1: Remove the top-level Linux-only test inclusion**

Delete the `if(CMAKE_SYSTEM_PROCESSOR ... add_subdirectory(test) ... endif()` block so the root build graph no longer references `test/`.

- [ ] **Step 2: Verify the root file still exposes the Windows build**

Confirm `enable_testing()` and `add_subdirectory(src)` remain unchanged so Windows selftests and GUI smoke registrations from `src/` still work.

### Task 2: Collapse CI to Windows

**Files:**
- Modify: `D:\Dev\Code\surakarta_traditional\.github\workflows\build-test.yml`
- Modify: `D:\Dev\Code\surakarta_traditional\.github\workflows\code-ql.yml`

- [ ] **Step 1: Reduce build-test matrix to Windows only**

Keep only `windows-latest` with the MSVC toolchain, remove Ubuntu entries, and update workflow comments/title so the file no longer claims to build Linux permutations.

- [ ] **Step 2: Move CodeQL analysis to Windows**

Replace the dynamic Ubuntu runner expression with `windows-latest` so the default analysis path matches the Windows-only support statement.

### Task 3: Rewrite Public Docs to Windows-Only

**Files:**
- Modify: `D:\Dev\Code\surakarta_traditional\README.md`

- [ ] **Step 1: Remove Linux support statements**

Delete the Linux compilation section and any wording that says the project supports Linux or UNIX-like builds.

- [ ] **Step 2: Describe the current Windows path accurately**

State that Visual Studio/MSVC is the maintained path, keep the Windows workflow commands, and update dependency wording so it no longer claims all dependencies are test-only when GUI builds still fetch `glfw` and `imgui`.

### Task 4: Verify the Closure

**Files:**
- Verify: `D:\Dev\Code\surakarta_traditional\CMakeLists.txt`
- Verify: `D:\Dev\Code\surakarta_traditional\.github\workflows\build-test.yml`
- Verify: `D:\Dev\Code\surakarta_traditional\.github\workflows\code-ql.yml`
- Verify: `D:\Dev\Code\surakarta_traditional\README.md`

- [ ] **Step 1: Inspect the final diff**

Run: `git diff -- CMakeLists.txt .github/workflows/build-test.yml .github/workflows/code-ql.yml README.md`

Expected: only Windows-only closure edits in those four files.

- [ ] **Step 2: Check the remaining top-level Linux references**

Run: `Select-String -Path README.md,.github/workflows/build-test.yml,.github/workflows/code-ql.yml,CMakeLists.txt -Pattern "Linux|ubuntu|UNIX-like" -SimpleMatch`

Expected: no matches in the edited files, except intentional historical references outside this scope.
