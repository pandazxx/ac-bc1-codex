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
- Topic 5 (Obstacle spawn logic): **Pure random with constraints for MVP**, plan to explore **Hybrid pattern + random variation** later.
  - MVP direction: randomize obstacle type/gap with safety constraints to avoid impossible sequences.
  - Later-phase direction: introduce curated pattern chunks and randomized micro-variation for better rhythm.
- Topic 6 (Score model): **Distance only** selected for MVP.
  - Implementation direction: score increases by survived distance/time only, with milestone feedback.
  - Later-phase option: add bonus systems (near-miss/combo) only after core feel is stable.
- Topic 7 (Visual direction): **Minimal color accent** selected.
  - Implementation direction: keep clean, high-contrast visuals with restrained accent colors for feedback and identity.
  - Asset implication: prioritize readability-first silhouettes before decorative detail.
- Topic 8 (MVP scope boundary): **Strict MVP** selected.
  - MVP includes: one biome, cactus+bird obstacles, distance score, game-over, and instant restart.
  - Deferred features: pause menu, high-score persistence, day/night cycle, and skins.
- Topic 9 (Technical baseline): **CMake-first** selected.
  - Implementation direction: establish a scalable CMake project layout from the start.
  - Immediate implications: standardize build presets/targets and keep testing hooks compatible with `ctest`.
- Topic 10 (First playable milestone): **Core-loop-first** selected.
  - Milestone includes: player controls, obstacle spawning, collision, game-over state, and instant restart.
  - Success criteria: a complete playable loop with tunable parameters and stable frame pacing.
- Topic 11 (Jump behavior tuning): **Low jump, fast fall** selected.
  - Implementation direction: use a snappy jump arc with strong downward acceleration for precision play.
  - Tuning priority: preserve input responsiveness while preventing floaty airtime.
- Topic 12 (Duck mechanic behavior): **Pure hitbox shrink** selected.
  - Implementation direction: duck only changes player profile/hurtbox, with no speed modifier.
  - Design rationale: keeps readability high and preserves strict rhythm-based timing.
- Topic 13 (Game-over restart UX): **Any-key restart** selected.
  - Implementation direction: allow immediate restart from game-over with any gameplay-relevant key.
  - UX rationale: minimize downtime and maximize rapid replay loops.
- Topic 14 (MVP audio policy): **Minimal SFX only** selected.
  - Implementation direction: include core feedback sounds (jump, hit, milestone/score cue) without background music.
  - Scope rationale: improves game feel while keeping MVP production cost low.
- Topic 15 (HUD scope for MVP): **Score + persistent best score** selected.
  - Implementation direction: display current score and all-time best score in HUD.
  - Technical implication: add lightweight local persistence for best score in MVP.

### Platform Discussion
- Platform Topic A (Primary MVP target): **Desktop + Web** selected for first release.
  - Implementation direction: keep core loop/input/render path compatible with native and HTML5/WebAssembly builds.
  - Follow-up needed: define persistence fallback behavior for web vs desktop.
- Platform Topic B (Web performance baseline): **60 FPS target with graceful degradation** selected.
  - Implementation direction: design for 60 FPS by default while preserving playable behavior under occasional frame drops.
  - Technical implication: keep update logic delta-time aware and avoid heavy per-frame allocations.
- Platform Topic C (Web input scheme): **Keyboard only** selected.
  - Implementation direction: keep input parity between desktop and web for MVP.
  - Scope implication: defer mouse/touch controls until post-MVP validation.
- Platform Topic D (Web deployment target): **S3 now, Itch.io end-goal** selected.
  - Current workflow: deploy web builds to S3 for early access/testing.
  - Final distribution goal: publish polished web build on Itch.io.
- Platform Topic E (S3 release model): **Versioned builds + latest alias** selected.
  - Implementation direction: publish immutable version directories and update a `latest/` pointer for quick access.
  - Ops benefit: supports rollback and reproducible testing while keeping a stable public link.
- Platform Topic F (Web persistence for best score): **Browser local storage only** selected.
  - Implementation direction: store best score client-side per browser/device for MVP.
  - Scope rationale: avoids backend/API complexity in early releases.
- Platform Topic G (Desktop distribution priority): **Web first**, Linux/macOS later.
  - Release priority: focus initial launch and iteration loop on web builds.
  - Desktop plan: add Linux/macOS native packaging in a later phase after web stabilization.
- Platform Topic H (Web analytics for MVP): **None** selected.
  - Implementation direction: ship without analytics instrumentation in MVP.
  - Feedback source: rely on direct playtest feedback and manual observation initially.
