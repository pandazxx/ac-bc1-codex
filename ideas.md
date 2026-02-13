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

### Topic-by-Topic Decisions
- Topic 1 (Core game feel): **Arcade-tight** selected.
  - Implementation direction: faster jump arc, stricter timing windows, strong speed escalation.
  - Follow-up needed: finalize hitbox strictness and input buffering policy to support this feel.
- Topic 2 (Hitbox policy): **Hybrid** selected.
  - Implementation direction: slightly forgiving player hurtbox, near-strict obstacle hitboxes.
  - Follow-up needed: choose exact shrink ratio (recommended start: player hurtbox at 85-90% of visual bounds).
- Topic 3 (Input buffering / coyote time): **Light** selected.
  - Implementation direction: add a small jump buffer and short coyote window to preserve responsiveness without removing challenge.
  - Initial tuning target: 75ms jump buffer, 50ms coyote time.
- Topic 4 (Difficulty ramp profile): **Stepped ramp** selected.
  - Implementation direction: increase speed in discrete jumps at score milestones for clear pacing shifts.
  - Initial tuning target: speed step every 100 points with capped max speed.
