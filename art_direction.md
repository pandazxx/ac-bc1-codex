# Art, Animation, and SFX Direction

## Purpose
This document defines the locked MVP creative direction for visual assets, animation behavior, and sound effects.

## Visual Art Direction (MVP)
- Primary style: pixel-inspired silhouettes.
- Rendering profile: mid-res crisp target (~480x270 style internal grid).
- Palette policy: day palette only.
- Background policy: minimal static background.
- Readability rules:
  - Keep obstacle silhouettes high-contrast against background.
  - Avoid noisy textures that reduce timing clarity.
  - Use accent color sparingly for player and feedback emphasis.

## Animation Direction (MVP)
- Player run: 4-frame loop.
- Jump/land: pose-based transitions with subtle squash/stretch.
- Bird obstacle: 3-frame wing flap.
- Hit feedback: brief freeze + very small camera shake.
- Animation pacing rules:
  - Keep transitions short to preserve arcade-tight response.
  - Prioritize silhouette clarity at high speeds over decorative motion.

## SFX Direction (MVP)
- Audio scope: SFX only (no music).
- Style: synthetic retro blips.
- Event set:
  - jump
  - land
  - hit
  - score milestone
  - restart
- Mix policy: even perceived loudness baseline across events.
- Variation policy: one stable sound per event.
- Runtime format: OGG.

## Asset Production Guidelines
- Create source assets in editable/lossless authoring formats.
- Export runtime assets to optimized web-friendly outputs.
- Keep frame counts and file sizes small for web-first performance.
- Validate assets in motion at gameplay speed before finalizing.

## Post-MVP Expansion (Deferred)
- Night palette variants.
- Background parallax layers.
- Additional SFX variations per event.
- Optional music layer.
