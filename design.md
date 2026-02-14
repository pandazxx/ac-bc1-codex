# Design Overview

## Vision
Build a polished, arcade-tight 2D runner inspired by Chrome Dino using raylib. The experience should prioritize immediate readability, rhythm mastery, and rapid replay loops (10-60 second runs with instant retry).

## Core Design Pillars
- Readability first: clear silhouettes, minimal UI noise, strong obstacle telegraphing.
- Precision rhythm: timing on jump/duck should feel skill-based and consistent.
- Fast iteration loop: short runs, immediate restart, low downtime.

## MVP Gameplay Specification
- Core loop: run -> avoid obstacles -> collide -> game over -> any-key restart.
- Controls (keyboard):
  - `Space` / `Up`: jump
  - `Down`: duck
- Obstacles: cactus variants (ground) and bird variants (air lanes).
- Difficulty: stepped ramp (speed increases every 100 score, capped max speed).
- Spawn model (MVP): pure random with safety constraints to prevent impossible sequences.
- Score model: distance only.
- HUD: current score + persistent best score.

## Feel and Tuning Targets
- Game feel: arcade-tight.
- Hitbox policy: hybrid
  - Player hurtbox slightly forgiving (start at ~85-90% of visual bounds).
  - Obstacles near-strict.
- Input forgiveness: light
  - Jump buffer: ~75ms
  - Coyote time: ~50ms
- Jump profile: low jump, fast fall.
- Duck behavior: hitbox/profile shrink only (no speed modifier).
- Audio scope (MVP): minimal SFX only (jump/hit/milestone), no background music.

## Visual Direction
- Style: minimal color accent (not monochrome-only).
- Priority: high-contrast readability over decorative detail.

## Platform and Release Strategy
- Primary release focus: web first.
- Native desktop: Linux/macOS later phase.
- Web performance target: 60 FPS with graceful degradation.
- Web input (MVP): keyboard only.
- Browser support target: latest Chrome, Firefox, Safari.

## Persistence and Deployment
- Best score persistence (web MVP): browser local storage.
- Deployment now: S3.
- S3 release model: versioned path `/<repo-name>/<version>/`.
- Version policy:
  - tag `release/<version>` -> deploy as `<version>`
  - otherwise deploy as `yyyy-mm-dd-<short-sha>`
- Distribution end-goal: publish to Itch.io.

## Tech and Delivery Baseline
- Language: C
- Build system: CMake-first
- First playable milestone: core-loop-first (player, obstacles, collision, game-over, restart)
- CI/CD direction:
  - CI on branch pushes (web build validation).
  - CD on `main` pushes and `release/*` tag pushes.
  - Publish web artifacts to `s3://<bucket>/<repo-name>/<version>/`.
  - Add cross-browser smoke checks before stable releases.

## Deferred (Post-MVP)
- Curated obstacle pattern system (hybrid spawn logic).
- Pause menu.
- Day/night cycle.
- Character skins.
- Advanced scoring bonuses (near-miss/combo).
- Analytics instrumentation.
