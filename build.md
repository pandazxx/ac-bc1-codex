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

Deployment packaging requirement:
- CD must publish `index.html` as the web entry page.
- `index.html` is generated from `build-web/raylib_dino_runner.html`.
- Deploy bundle shape:
  - `index.html`
  - `*.js`
  - `*.wasm`

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
- Upload each release to `s3://<bucket>/<repo-name>/<version>/`.
- `<repo-name>` is the GitHub repository name (right side of `owner/repo`).
- `<version>` is resolved as:
  - If the push is tag `release/<version>`, use `<version>`.
  - Otherwise use `yyyy-mm-dd-<short-sha>` (UTC date + short commit hash).
- Keep versioned artifacts immutable for rollback and traceability.
- Entry page contract: each deployed version must include `index.html` at the root of that version prefix.

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
   - `s3://$S3_BUCKET/$REPO_NAME/$VERSION/`

Recommended hardening:
- Replace static IAM keys with GitHub OIDC role assumption when ready.

## CI/CD Expectations
- CI trigger: any branch push.
  - Current CI scope: web configure/build artifact validation.
- CD trigger:
  - push to `main`, or
  - push of tag matching `release/*`.
- CD deploy target: `s3://$S3_BUCKET/$REPO_NAME/$VERSION/`.
- CD publish content: `index.html`, JS, and WASM (entry path is `/<repo-name>/<version>/index.html`).
- Browser validation target: latest Chrome, Firefox, Safari.
