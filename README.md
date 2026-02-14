# Raylib Dino Runner

Arcade-tight 2D runner inspired by Chrome Dino, built in C with raylib. And run on Web.

## Current Status
- Core playable loop implemented: run, avoid obstacles, collision, game over, any-key restart.
- Art direction integration: sprite sheets, animation states, OGG SFX events, and hit feedback.
- Keyboard controls: `Space`/`Up` jump, `Down` duck.
- Debug controls: `D` toggles hitbox overlay.
- Score system: distance score + persistent best score.
- Platform focus: web first (desktop later).
- Deployment entry page: `index.html` per versioned S3 release path.

## Project Layout
- `src/`: gameplay and runtime code
- `include/`: headers and module interfaces
- `assets/runtime/`: runtime sprite and SFX assets used by desktop/web builds
- `design.md`: consolidated game design decisions
- `art_direction.md`: locked MVP art/animation/SFX direction
- `docs/test/manualtestcase/debug_input.md`: manual debug-input QA cases
- `ideas.md`: running discussion log and open topics
- `build.md`: build/run/deploy instructions

## Quick Start
See `build.md` for full setup. Common flow:

```bash
cmake -S . -B build
cmake --build build
./build/raylib_dino_runner
```

## Controls
- `Space` or `Up`: Jump
- `Down`: Duck
- Any key on game-over: Restart
- `D`: Toggle hitbox debug mode

## Next Milestones
- Tune obstacle pacing and jump/duck feel from playtest feedback.
- Add automated tests (`ctest`) for deterministic gameplay logic.
- Add CI/CD workflow for S3 web artifact publishing.
