#!/usr/bin/env python3
from pathlib import Path
from PIL import Image, ImageDraw

FRAME = 32
PLAYER_SIZE = (128, 64)
OBS_SIZE = (192, 32)

SETS = {
    "set_a": {
        "name": "Bold Contrast",
        "body": (28, 28, 28, 255),
        "shade": (70, 70, 70, 255),
        "eye": (242, 107, 58, 255),
        "cactus": (31, 58, 33, 255),
        "cactus_hi": (58, 98, 62, 255),
        "bird": (237, 98, 54, 255),
        "bird_dark": (175, 61, 32, 255),
    },
    "set_b": {
        "name": "Retro Soft",
        "body": (45, 41, 39, 255),
        "shade": (87, 77, 70, 255),
        "eye": (233, 143, 74, 255),
        "cactus": (53, 74, 48, 255),
        "cactus_hi": (82, 108, 73, 255),
        "bird": (212, 120, 75, 255),
        "bird_dark": (145, 83, 49, 255),
    },
    "set_c": {
        "name": "Arcade Punch",
        "body": (18, 18, 24, 255),
        "shade": (77, 63, 112, 255),
        "eye": (255, 92, 58, 255),
        "cactus": (20, 68, 50, 255),
        "cactus_hi": (63, 130, 100, 255),
        "bird": (255, 120, 77, 255),
        "bird_dark": (166, 62, 37, 255),
    },
}


def rect(draw, x, y, w, h, c):
    draw.rectangle([x, y, x + w - 1, y + h - 1], fill=c)


def draw_dino(draw, p, ox=0, oy=0, crouch=False, squash=0, leg_pose=0):
    body_y = oy + 12 + squash
    body_h = 11 - squash

    rect(draw, ox + 10, body_y, 13, body_h, p["body"])
    rect(draw, ox + 18, body_y - 9, 8, 9, p["body"])
    rect(draw, ox + 24, body_y - 6, 4, 6, p["body"])
    rect(draw, ox + 21, body_y - 6, 1, 1, p["eye"])
    rect(draw, ox + 12, body_y - 2, 8, 2, p["shade"])

    if crouch:
        rect(draw, ox + 7, oy + 20, 18, 7, p["body"])
        rect(draw, ox + 22, oy + 16, 7, 6, p["body"])
        rect(draw, ox + 10, oy + 27, 6, 3, p["body"])
        rect(draw, ox + 18, oy + 27, 6, 3, p["body"])
        return

    if leg_pose == 0:
        rect(draw, ox + 11, oy + 23, 4, 6, p["body"])
        rect(draw, ox + 17, oy + 26, 4, 3, p["body"])
    elif leg_pose == 1:
        rect(draw, ox + 11, oy + 26, 4, 3, p["body"])
        rect(draw, ox + 17, oy + 23, 4, 6, p["body"])
    elif leg_pose == 2:
        rect(draw, ox + 11, oy + 24, 4, 5, p["body"])
        rect(draw, ox + 17, oy + 26, 4, 3, p["body"])
    else:
        rect(draw, ox + 11, oy + 26, 4, 3, p["body"])
        rect(draw, ox + 17, oy + 24, 4, 5, p["body"])


def draw_player_sheet(path: Path, p):
    img = Image.new("RGBA", PLAYER_SIZE, (0, 0, 0, 0))
    d = ImageDraw.Draw(img)

    draw_dino(d, p, 0, 0, leg_pose=0)
    draw_dino(d, p, 32, 0, leg_pose=1)
    draw_dino(d, p, 64, 0, leg_pose=2)
    draw_dino(d, p, 96, 0, leg_pose=3)

    draw_dino(d, p, 0, 32, leg_pose=2)
    draw_dino(d, p, 32, 32, crouch=True)
    draw_dino(d, p, 64, 32, squash=2, leg_pose=0)
    draw_dino(d, p, 96, 32, leg_pose=1)

    img.save(path)


def cactus(draw, p, ox, variant):
    if variant == 0:
        rect(draw, ox + 14, 9, 4, 21, p["cactus"])
        rect(draw, ox + 11, 16, 3, 6, p["cactus"])
        rect(draw, ox + 18, 12, 3, 7, p["cactus"])
    elif variant == 1:
        rect(draw, ox + 13, 6, 5, 24, p["cactus"])
        rect(draw, ox + 9, 15, 4, 6, p["cactus"])
        rect(draw, ox + 18, 18, 4, 7, p["cactus"])
    else:
        rect(draw, ox + 12, 10, 8, 20, p["cactus"])
        rect(draw, ox + 9, 18, 3, 7, p["cactus"])
        rect(draw, ox + 20, 13, 3, 6, p["cactus"])
    rect(draw, ox + 14, 12, 1, 14, p["cactus_hi"])


def bird(draw, p, ox, wing):
    rect(draw, ox + 9, 13, 15, 6, p["bird"])
    rect(draw, ox + 24, 14, 3, 3, p["bird_dark"])
    rect(draw, ox + 12, 12, 2, 1, p["bird_dark"])
    if wing == 0:
        rect(draw, ox + 12, 8, 8, 4, p["bird"])
    elif wing == 1:
        rect(draw, ox + 12, 11, 8, 3, p["bird"])
    else:
        rect(draw, ox + 12, 18, 8, 4, p["bird"])


def draw_obstacle_sheet(path: Path, p):
    img = Image.new("RGBA", OBS_SIZE, (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    cactus(d, p, 0, 0)
    cactus(d, p, 32, 1)
    cactus(d, p, 64, 2)
    bird(d, p, 96, 0)
    bird(d, p, 128, 1)
    bird(d, p, 160, 2)
    img.save(path)


def main():
    base = Path("assets/review_sets")
    base.mkdir(parents=True, exist_ok=True)

    for key, cfg in SETS.items():
        out = base / key
        out.mkdir(parents=True, exist_ok=True)
        draw_player_sheet(out / "player.png", cfg)
        draw_obstacle_sheet(out / "obstacles.png", cfg)

    notes = base / "README.md"
    notes.write_text(
        "# Art Review Sets\n\n"
        "All sets keep engine-compatible dimensions:\n"
        "- player.png: 128x64 (8 frames, 32x32 each)\n"
        "- obstacles.png: 192x32 (6 frames, 32x32 each)\n\n"
        "## Sets\n"
        "- set_a: Bold Contrast\n"
        "- set_b: Retro Soft\n"
        "- set_c: Arcade Punch\n",
        encoding="utf-8",
    )

    print("Generated 3 review sets in assets/review_sets")


if __name__ == "__main__":
    main()
