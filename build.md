# Build and Deployment Guide

## Prerequisites

### Desktop
- C compiler with C99 support
- CMake 3.20+
- raylib installed and discoverable by CMake (`find_package(raylib REQUIRED)`)

### Web
- Emscripten SDK activated (`emcmake`, `emmake` available)
- raylib web build support in your Emscripten environment

## Build (Desktop)

```bash
cmake -S . -B build
cmake --build build
./build/raylib_dino_runner
```

## Build (Web / WebAssembly)

```bash
emcmake cmake -S . -B build-web
cmake --build build-web
```

Expected output: `build-web/raylib_dino_runner.html` (plus `.js`/`.wasm`).

## Local Web Smoke Test
Use any static server from project root, for example:

```bash
python3 -m http.server 8080
```

Then open the generated web build in browser and verify:
- startup + render
- jump/duck input
- collision/game-over/restart
- best score persistence in same browser

## S3 Publish Model (Current)
- Upload each release to a versioned prefix (example: `s3://<bucket>/game/v0.1.0/`).
- Update `s3://<bucket>/game/latest/` to point to current public build.
- Keep versioned artifacts immutable for rollback.

## CI/CD Expectations
- CI: build + tests on PRs and pushes.
- CD: on merge/tag, publish web artifacts to S3 versioned path and refresh `latest/`.
- Browser validation target: latest Chrome, Firefox, Safari.
