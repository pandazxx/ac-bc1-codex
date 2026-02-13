# Repository Guidelines

## Project Structure & Module Organization
This repository is currently minimal and contains only `README.md`. As the Raylib Dino Runner codebase is added, use this layout:
- `src/`: game code (e.g., `main.c`, gameplay modules, rendering)
- `include/`: public headers shared across modules
- `assets/`: sprites, fonts, audio, and other runtime resources
- `tests/`: automated tests and test fixtures
- `build/`: local build outputs (do not commit)

Keep modules focused by responsibility (example: `src/player.c`, `src/obstacles.c`, `src/collision.c`).

## Build, Test, and Development Commands
No build system is committed yet. When adding one, expose standard commands and document them in `README.md`:
- `make build`: compile the game
- `make run`: build and run locally
- `make test`: run automated tests
- `make clean`: remove artifacts from `build/`

If you use CMake instead, provide equivalent commands (for example: `cmake -S . -B build && cmake --build build`).

## Coding Style & Naming Conventions
Use consistent C conventions:
- Indentation: 4 spaces, no tabs
- File names: lowercase snake_case (`game_state.c`, `game_state.h`)
- Functions/variables: snake_case
- Macros/constants: UPPER_SNAKE_CASE
- Header guards: `PROJECT_MODULE_H`

Prefer small functions, explicit ownership of resources, and module-local `static` helpers where possible.

## Testing Guidelines
Add tests under `tests/` with names ending in `_test.c` (example: `collision_test.c`). Focus on deterministic logic (collision checks, scoring, spawn timing). Run tests before each PR and keep gameplay-critical paths covered.

## Commit & Pull Request Guidelines
There is no commit history yet, so adopt Conventional Commits from the start:
- `feat: add obstacle spawn system`
- `fix: prevent jump double-trigger`
- `docs: update build instructions`

PRs should include:
- Clear summary of behavior changes
- Linked issue (`Closes #<id>`) when applicable
- Test evidence (command output or short notes)
- Screenshot/GIF for visible gameplay changes

## Security & Configuration Tips
Do not commit secrets, machine-specific paths, or compiled binaries. Keep large assets optimized and store only source assets needed for development/runtime.

## Agent-Specific Instructions
- Call the user `Moses`.
- Start every conversation with `What's up Moses.`
- `ideas.md` records every topic and idea we discuss, plus open items and TODOs; use it to guide implementation.
- For every conversation, if files change, commit and push to `origin`.
