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

## S3 Credentials Setup (GitHub Actions)
1. Create an IAM user for CI/CD deploys.
2. Grant least-privilege access to your target bucket/prefix:
   - `s3:ListBucket`
   - `s3:GetObject`
   - `s3:PutObject`
   - `s3:DeleteObject`
3. Create an access key for that IAM user.
4. In GitHub: `Settings -> Secrets and variables -> Actions`, add:
   - `AWS_ACCESS_KEY_ID`
   - `AWS_SECRET_ACCESS_KEY`
   - `AWS_REGION` (example: `us-east-1`)
   - `S3_BUCKET` (bucket name only, no `s3://`)
5. Re-run the workflow; `deploy-s3` will sync web artifacts to:
   - `s3://$S3_BUCKET/game/latest/`

Recommended hardening:
- Replace static IAM keys with GitHub OIDC role assumption when ready.

## CI/CD Expectations
- CI: build + tests on PRs and pushes.
- CD: on merge/tag, publish web artifacts to S3 versioned path and refresh `latest/`.
- Browser validation target: latest Chrome, Firefox, Safari.
