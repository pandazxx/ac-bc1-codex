# Ideas Log

## Implemented Decisions

### Product Direction
- Build an arcade-tight 2D runner inspired by Chrome Dino.
- Prioritize fast restart loops and short-session replayability.
- Keep readability-first visuals and gameplay feedback.

### Core Gameplay
- Controls:
  - `Space`/`Up`: jump
  - `Down`: duck
  - any key on game-over: restart
  - `D`: toggle debug hitboxes
- MVP scope is strict:
  - one biome
  - cactus + bird obstacles
  - distance score
  - game-over and instant restart
  - persistent best score
- Score model is distance-only for MVP.
- Difficulty uses stepped speed increases by score milestones.

### Game Feel Tuning
- Jump behavior: low jump with fast fall.
- Input forgiveness: light jump buffer + short coyote time.
- Duck behavior: hitbox/profile shrink only; no speed modifier.
- Hitbox policy: hybrid (slightly forgiving player, near-strict obstacles).
- Hit feedback: short freeze + tiny camera shake.

### Visual and Audio Direction
- Visual style: pixel-inspired silhouettes with modern readability.
- Palette strategy: single day palette for MVP.
- Background treatment: minimal static background.
- Player run cycle: 4 frames.
- Bird animation: 3-frame flap.
- Jump/land animation language: pose-based with small squash/stretch.
- SFX scope: minimal event-driven SFX, no BGM.
- SFX style: synthetic retro blips.
- SFX variation policy: single sound per event.
- Runtime audio format: OGG.

### Platform and Runtime
- Platform priority: web first, desktop later.
- MVP targets: desktop + web compatibility in code path.
- Web input scheme: keyboard-first.
- Web performance target: 60 FPS with graceful degradation.
- Browser support target: latest Chrome, Firefox, Safari.
- Web high-score persistence: browser local storage.
- Fixed-step simulation at 60 Hz with decoupled rendering.

### Build, Test, and Delivery
- Build system baseline: CMake-first.
- Include `ctest` integration from the start.
- CI scope currently focuses on web build validation.
- CD deploys versioned artifacts to `s3://<bucket>/<repo-name>/<version>/`.
- Version resolution:
  - `release/<version>` tag uses `<version>`
  - otherwise use `yyyy-mm-dd-<short-sha>`
- Deployment contract includes `index.html` at each version root.
- CD also publishes deployment listings at repo root:
  - `/<repo-name>/index.html`
  - `/<repo-name>/deployments.json`

### Implemented Systems and Assets
- Core modules implemented:
  - `main`, `game`, `player`, `obstacle`, `score_store`
  - `animation`, `art_assets`, `audio_events`
- Automated test implemented:
  - `animation_test`
- Runtime assets integrated:
  - player/obstacle sprite sheets
  - jump/land/hit/milestone/restart OGG SFX
- Active runtime sprite set: `set_d` (Donkey Runner).

### Baseline Version
- Baseline is treated as V1 at commit `ff27383` (`origin/master` snapshot).

## Backlog Decisions

### Gameplay and Content
- Evolve spawn logic from pure random constraints to hybrid pattern + random variation.
- Add day/night palette cycle after MVP stabilization.
- Add unlockable character skins tied to score milestones.
- Add daily challenge seed mode.
- Add optional assist mode (slower acceleration) for accessibility.

### Quality and Testing
- Expand deterministic tests beyond animation:
  - collision edge cases
  - obstacle spawn safety/timing constraints
  - score progression boundaries
- Add broader gameplay-critical coverage before larger feature expansion.

### Platform and Release
- Keep S3 for iteration; publish polished build to Itch.io later.
- Add Linux/macOS packaging after web stabilization.
- Keep analytics out of MVP; reconsider post-MVP.
- Improve web loading optimization after gameplay/deployment stability.

### Infrastructure and Security
- Replace static IAM key usage in CI/CD with GitHub OIDC role assumption.

### Environment Gaps
- Current local environment still needs `cmake` installed for full configure/build/test execution.
