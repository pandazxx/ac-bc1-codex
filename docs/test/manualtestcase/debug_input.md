# Debug Input Manual Test Cases

## Test Environment
- Browser: Chrome (latest)
- URL: deployed web build (`/<repo>/<version>/index.html`)
- Device: desktop keyboard

## Preconditions
- Game loads successfully
- Canvas is visible and interactive
- Click canvas once before running tests

## TC-DBG-001: Single Press Toggle
- Steps:
  1. Press `D` once
- Expected:
  - Hitbox overlay turns on (green player hurtbox + red obstacle boxes)

## TC-DBG-002: Rapid Repeated Toggle
- Steps:
  1. Press `D` 5 times quickly
- Expected:
  - Overlay toggles each press and ends in ON state (odd toggles)

## TC-DBG-003: Double Tap Parity
- Steps:
  1. Press `D` twice quickly
- Expected:
  - Overlay returns to previous state (even toggles)

## TC-DBG-004: F3 Alternate Toggle
- Steps:
  1. Press `F3`
- Expected:
  - Overlay toggles same as `D`

## TC-DBG-005: Focus Drift Recovery
- Steps:
  1. Click outside canvas/browser chrome
  2. Click back into page
  3. Press `D`
- Expected:
  - Overlay still toggles reliably

## TC-DBG-006: Jump Input Non-Regression
- Steps:
  1. Press `Space` or `Up` repeatedly
- Expected:
  - Jump responsiveness remains unchanged
  - Debug toggle changes do not affect jump timing

## Expected Results Matrix
- `D` / `F3` should be parity-correct:
  - odd count -> state flips
  - even count -> state unchanged

## Known Notes
- Browser hotkeys/extensions may intercept `F3`; use `D` as primary toggle.
- Audio may require first user interaction due to browser autoplay policy.
