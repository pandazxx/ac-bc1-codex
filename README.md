# Raylib Dino Runner

Arcade-tight 2D runner inspired by Chrome Dino, built in C with raylib. And run on Web.

## Current Status
- Core playable loop implemented: run, avoid obstacles, collision, game over, any-key restart.
- Keyboard controls: `Space`/`Up` jump, `Down` duck.
- Score system: distance score + persistent best score.
- Platform focus: web first (desktop later).
- Deployment entry page: `index.html` per versioned S3 release path.

## Project Layout
- `src/`: gameplay and runtime code
- `include/`: headers and module interfaces
- `design.md`: consolidated game design decisions
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

## Next Milestones
- Tune obstacle pacing and jump/duck feel from playtest feedback.
- Add automated tests (`ctest`) for deterministic gameplay logic.
- Add CI/CD workflow for S3 web artifact publishing.
