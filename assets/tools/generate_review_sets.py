#!/usr/bin/env python3
from pathlib import Path
from PIL import Image, ImageDraw

PLAYER_SIZE = (128, 64)
OBS_SIZE = (192, 32)

SETS = {
    "set_a": {
        "name": "Caveman Club",
        "skin": (201, 150, 112, 255),
        "hair": (67, 44, 30, 255),
        "cloth": (166, 94, 57, 255),
        "tool": (128, 95, 65, 255),
        "tool_hi": (166, 127, 86, 255),
        "eye": (22, 22, 22, 255),
        "cactus": (31, 58, 33, 255),
        "cactus_hi": (58, 98, 62, 255),
        "bird": (237, 98, 54, 255),
        "bird_dark": (175, 61, 32, 255),
        "style": "club",
    },
    "set_b": {
        "name": "Fur Runner",
        "skin": (188, 136, 104, 255),
        "hair": (52, 52, 56, 255),
        "cloth": (86, 98, 116, 255),
        "tool": (130, 130, 138, 255),
        "tool_hi": (166, 166, 176, 255),
        "eye": (250, 228, 184, 255),
        "cactus": (53, 74, 48, 255),
        "cactus_hi": (82, 108, 73, 255),
        "bird": (212, 120, 75, 255),
        "bird_dark": (145, 83, 49, 255),
        "style": "hood",
    },
    "set_c": {
        "name": "Spear Scout",
        "skin": (212, 162, 120, 255),
        "hair": (34, 34, 42, 255),
        "cloth": (55, 111, 92, 255),
        "tool": (112, 86, 56, 255),
        "tool_hi": (168, 138, 94, 255),
        "eye": (255, 96, 74, 255),
        "cactus": (20, 68, 50, 255),
        "cactus_hi": (63, 130, 100, 255),
        "bird": (255, 120, 77, 255),
        "bird_dark": (166, 62, 37, 255),
        "style": "spear",
    },
    "set_d": {
        "name": "Donkey Runner",
        "skin": (142, 120, 102, 255),
        "hair": (72, 60, 52, 255),
        "cloth": (108, 92, 82, 255),
        "tool": (95, 80, 66, 255),
        "tool_hi": (140, 120, 100, 255),
        "eye": (250, 242, 230, 255),
        "cactus": (36, 66, 48, 255),
        "cactus_hi": (80, 130, 97, 255),
        "bird": (236, 112, 84, 255),
        "bird_dark": (154, 74, 49, 255),
        "style": "donkey",
    },
}


def rect(draw, x, y, w, h, c):
    draw.rectangle([x, y, x + w - 1, y + h - 1], fill=c)


def draw_legs(draw, p, ox, oy, pose):
    if pose == 0:
        rect(draw, ox + 12, oy + 22, 4, 7, p["skin"])
        rect(draw, ox + 18, oy + 25, 4, 4, p["skin"])
    elif pose == 1:
        rect(draw, ox + 12, oy + 25, 4, 4, p["skin"])
        rect(draw, ox + 18, oy + 22, 4, 7, p["skin"])
    elif pose == 2:
        rect(draw, ox + 12, oy + 23, 4, 6, p["skin"])
        rect(draw, ox + 18, oy + 26, 4, 3, p["skin"])
    else:
        rect(draw, ox + 12, oy + 26, 4, 3, p["skin"])
        rect(draw, ox + 18, oy + 23, 4, 6, p["skin"])


def draw_caveman(draw, p, ox=0, oy=0, pose=0, crouch=False, squash=0, airborne=False):
    torso_y = oy + 11 + squash
    torso_h = 10 - squash

    if crouch:
        rect(draw, ox + 8, oy + 20, 18, 7, p["cloth"])
        rect(draw, ox + 20, oy + 15, 7, 6, p["skin"])
        rect(draw, ox + 10, oy + 27, 6, 3, p["skin"])
        rect(draw, ox + 18, oy + 27, 6, 3, p["skin"])
    else:
        rect(draw, ox + 10, torso_y, 13, torso_h, p["cloth"])
        rect(draw, ox + 18, torso_y - 7, 8, 7, p["skin"])
        rect(draw, ox + 24, torso_y - 4, 4, 4, p["skin"])
        rect(draw, ox + 21, torso_y - 5, 1, 1, p["eye"])
        if not airborne:
            draw_legs(draw, p, ox, oy, pose)
        else:
            rect(draw, ox + 12, oy + 24, 4, 4, p["skin"])
            rect(draw, ox + 18, oy + 24, 4, 4, p["skin"])

    # hair/hood silhouettes
    if p["style"] == "club":
        rect(draw, ox + 17, (oy + 2 if not crouch else oy + 12), 9, 3, p["hair"])
        if not crouch:
            rect(draw, ox + 18, oy + 6, 2, 4, p["hair"])
    elif p["style"] == "hood":
        rect(draw, ox + 16, (oy + 1 if not crouch else oy + 10), 11, 5, p["hair"])
    else:  # spear scout headband
        rect(draw, ox + 17, (oy + 3 if not crouch else oy + 12), 10, 2, p["hair"])

    # tools/accessories for silhouette differentiation
    if p["style"] == "club" and not crouch:
        rect(draw, ox + 6, oy + 12, 2, 11, p["tool"])
        rect(draw, ox + 5, oy + 11, 4, 3, p["tool_hi"])
    elif p["style"] == "hood" and not crouch:
        rect(draw, ox + 7, oy + 15, 3, 8, p["tool"])
        rect(draw, ox + 6, oy + 14, 5, 2, p["tool_hi"])
    elif p["style"] == "spear":
        rect(draw, ox + 4, (oy + 8 if not crouch else oy + 14), 1, 17, p["tool"])
        rect(draw, ox + 3, (oy + 7 if not crouch else oy + 13), 3, 2, p["tool_hi"])


def draw_player_sheet(path: Path, p):
    img = Image.new("RGBA", PLAYER_SIZE, (0, 0, 0, 0))
    d = ImageDraw.Draw(img)

    if p["style"] == "donkey":
        draw_donkey_sheet(d, p)
    else:
        draw_caveman(d, p, 0, 0, pose=0)
        draw_caveman(d, p, 32, 0, pose=1)
        draw_caveman(d, p, 64, 0, pose=2)
        draw_caveman(d, p, 96, 0, pose=3)

        draw_caveman(d, p, 0, 32, pose=2, airborne=True)
        draw_caveman(d, p, 32, 32, crouch=True)
        draw_caveman(d, p, 64, 32, pose=0, squash=2)
        draw_caveman(d, p, 96, 32, pose=1, airborne=True)

    img.save(path)


def draw_donkey(draw, p, ox=0, oy=0, stride=0, crouch=False, squat=False, airborne=False):
    body = p["skin"]
    dark = p["hair"]
    eye = p["eye"]

    body_y = oy + (12 if not squat else 14)
    body_h = 9 if not squat else 7
    rect(draw, ox + 8, body_y, 16, body_h, body)   # torso
    rect(draw, ox + 22, body_y - 5, 7, 6, body)    # neck/head
    rect(draw, ox + 28, body_y - 3, 2, 3, dark)    # snout tip
    rect(draw, ox + 24, body_y - 8, 2, 4, dark)    # ear1
    rect(draw, ox + 27, body_y - 8, 2, 4, dark)    # ear2
    rect(draw, ox + 24, body_y - 4, 1, 1, eye)     # eye
    rect(draw, ox + 6, body_y + 1, 2, 1, dark)     # tail root
    rect(draw, ox + 4, body_y + 1, 2, 1, dark)     # tail tip

    if crouch:
        rect(draw, ox + 10, oy + 24, 6, 4, body)
        rect(draw, ox + 17, oy + 24, 6, 4, body)
        return

    if airborne:
        rect(draw, ox + 10, oy + 22, 4, 4, body)
        rect(draw, ox + 18, oy + 22, 4, 4, body)
        return

    # hoof poses
    if stride == 0:
        rect(draw, ox + 10, oy + 20, 3, 8, body)
        rect(draw, ox + 18, oy + 24, 3, 4, body)
    elif stride == 1:
        rect(draw, ox + 10, oy + 24, 3, 4, body)
        rect(draw, ox + 18, oy + 20, 3, 8, body)
    elif stride == 2:
        rect(draw, ox + 10, oy + 21, 3, 7, body)
        rect(draw, ox + 18, oy + 24, 3, 4, body)
    else:
        rect(draw, ox + 10, oy + 24, 3, 4, body)
        rect(draw, ox + 18, oy + 21, 3, 7, body)


def draw_donkey_sheet(draw, p):
    draw_donkey(draw, p, 0, 0, stride=0)
    draw_donkey(draw, p, 32, 0, stride=1)
    draw_donkey(draw, p, 64, 0, stride=2)
    draw_donkey(draw, p, 96, 0, stride=3)

    draw_donkey(draw, p, 0, 32, stride=2, airborne=True)
    draw_donkey(draw, p, 32, 32, crouch=True)
    draw_donkey(draw, p, 64, 32, stride=0, squat=True)
    draw_donkey(draw, p, 96, 32, stride=1, airborne=True)


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
        "- set_a: Caveman Club\n"
        "- set_b: Fur Runner\n"
        "- set_c: Spear Scout\n"
        "- set_d: Donkey Runner\n",
        encoding="utf-8",
    )

    print("Generated 4 review sets in assets/review_sets")


if __name__ == "__main__":
    main()
