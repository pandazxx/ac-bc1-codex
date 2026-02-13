# Ideas Log

## Session 2026-02-13

### Topics & Ideas Discussed
- Core direction: a polished Raylib runner inspired by Chrome Dino, with tight controls and fast restart loops.
- Design pillar 1: immediate readability (clear obstacles, simple silhouettes, minimal UI).
- Design pillar 2: mastery through rhythm (jump timing, duck timing, speed scaling).
- Design pillar 3: short-session replayability (10-60 second runs, instant retry).

### Gameplay Brainstorm
- Controls: `Space`/`Up` jump, `Down` duck, `R` restart.
- Core obstacles: cactus variants (low), bird variants (mid/high lanes).
- Difficulty curve: increase scroll speed over time, then add denser obstacle patterns.
- Score model: distance-based score + milestone badges every 100 points.
- States: title, run, game over, paused.

### Feature Ideas (Post-MVP)
- Day/night cycle affecting palette and obstacle contrast.
- Character skins unlocked via high score milestones.
- Daily challenge seed (same obstacle sequence for all players that day).
- Lightweight assist mode (slower acceleration) for accessibility.

### Open Items & TODOs
- Decide visual direction: monochrome retro vs. minimal color accent.
- Define final hitbox policy: forgiving vs. strict pixel-approximate.
- Choose build/test stack (`Makefile` only or `CMake` + `ctest`).
- Write first implementation tasks:
  - Bootstrap `src/main.c` game loop and state machine.
  - Add `player` module (jump physics, duck state).
  - Add `obstacle` module (spawn, movement, despawn).
  - Add collision checks + score tracking.
  - Add restart flow and game-over UI.
