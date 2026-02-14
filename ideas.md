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
  - Status: superseded by later CI/CD update to `/<repo-name>/<version>/` deploy paths.
  - Historical rationale: versioned artifacts support rollback and reproducible testing.
- Platform Topic F (Web persistence for best score): **Browser local storage only** selected.
  - Implementation direction: store best score client-side per browser/device for MVP.
  - Scope rationale: avoids backend/API complexity in early releases.
- Platform Topic G (Desktop distribution priority): **Web first**, Linux/macOS later.
  - Release priority: focus initial launch and iteration loop on web builds.
  - Desktop plan: add Linux/macOS native packaging in a later phase after web stabilization.
- Platform Topic H (Web analytics for MVP): **None** selected.
  - Implementation direction: ship without analytics instrumentation in MVP.
  - Feedback source: rely on direct playtest feedback and manual observation initially.
- Platform Topic I (S3 release channel): **Single public channel (`latest`)** selected.
  - Status: superseded by later CI/CD update to explicit versioned release paths per deploy.
  - Historical rationale: simplify early release operations.
- Platform Topic J (Web build loading strategy): **Iteration-first** selected.
  - Implementation direction: keep MVP pipeline simple with minimal load-time optimization initially.
  - Follow-up plan: optimize compression/bundling after gameplay stability and deployment flow are validated.
- Platform Topic K (Browser support target): **Latest Chrome + Firefox + Safari** selected.
  - Implementation direction: validate MVP builds across all three major browsers before release.
  - QA implication: include cross-browser smoke checks in the release checklist.

### Implementation Notes
- Implemented first playable skeleton with CMake, raylib modules, and gameplay loop:
  - `src/main.c`, `src/game.c`, `src/player.c`, `src/obstacle.c`, `src/score_store.c`
  - `include/game.h`, `include/player.h`, `include/obstacle.h`, `include/score_store.h`
- Added documentation and pipeline scaffolding:
  - `design.md` (consolidated spec), `build.md` (build/deploy guide), updated `README.md`
  - `.github/workflows/ci-cd.yml` for CI + web artifact + S3 latest sync
- Environment gap in this workspace: `cmake` is not installed locally, so configure/build could not be executed here.

### CI/CD Updates
- Removed desktop build from GitHub Actions for now.
- CI now runs on branch pushes (web build validation only).
- CD now runs on:
  - pushes to `main` or `master`
  - tag pushes matching `release/*`
- S3 deploy path changed to `/$repo-name/$version`, where:
  - `$repo-name` is derived from the GitHub repository name
  - `$version` is tag suffix for `release/<version>` or `yyyy-mm-dd-<short-sha>` otherwise
- CD packaging requirement added:
  - publish `index.html` as the entry page for each deployed version
  - map `build-web/raylib_dino_runner.html` to `index.html` before S3 sync

### Art / Animation / SFX Brainstorm (Kickoff)
- Goal: define a cohesive aesthetic package that preserves readability and arcade-tight feel.
- Constraints from prior decisions:
  - Minimal color accent visual direction
  - Web-first performance target (60 FPS with graceful degradation)
  - MVP audio scope is SFX only (no BGM)

#### Candidate Art Directions
- Direction A: clean flat silhouettes, warm accent (orange/red), subtle parallax bands.
- Direction B: pixel-art inspired silhouettes with modern UI text and smooth camera feel.
- Direction C: low-poly vector look with slightly exaggerated obstacle shapes.

#### Candidate Animation Language
- Player: 2-3 frame run cycle, snappy jump takeoff frame, squash on landing.
- Obstacles: mostly static readability-first; birds get 2-frame flap.
- Feedback: short hit freeze (40-60ms), quick score milestone pop.

#### Candidate SFX Set (MVP)
- `jump`: short clicky whoosh
- `land`: light thud
- `hit`: sharp muted impact
- `score_milestone`: concise UI ping
- `restart`: soft confirm tick

#### Open Art Pipeline Items
- Decide source style: vector-first vs pixel-first.
- Decide base resolution and scaling policy for web.
- Define palette tokens (bg/fg/accent/warn).

### Art Topic Decisions
- Art Topic 1 (Primary visual direction): **Pixel-inspired silhouettes** selected.
  - Direction: retro-flavored shapes with modern readability priorities.
  - Implication: keep sprite counts low and silhouettes clear to protect web performance.
- Art Topic 2 (Pixel density/base resolution): **Mid-res crisp** selected.
  - Direction: target a ~480x270 style internal pixel grid with clean upscale behavior.
  - Implication: better silhouette readability while retaining retro texture.
- Art Topic 3 (Palette strategy): **Day palette only (MVP)** selected.
  - Direction: single polished daytime palette for reliable readability and faster production.
  - Deferred: night palette variants and dynamic cycling post-MVP.
