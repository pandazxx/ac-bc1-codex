#!/usr/bin/env python3
from PIL import Image, ImageDraw

PALETTE = {
    "bg": (0, 0, 0, 0),
    "body": (33, 33, 33, 255),
    "shade": (65, 65, 65, 255),
    "eye": (242, 107, 58, 255),
    "bird": (242, 107, 58, 255),
    "bird_dark": (194, 78, 36, 255),
    "cactus": (34, 58, 34, 255),
    "cactus_hi": (52, 84, 52, 255),
}


def rect(draw, x, y, w, h, c):
    draw.rectangle([x, y, x + w - 1, y + h - 1], fill=c)


def dino_base(draw, ox=0, oy=0, crouch=False, squash=0):
    body_y = oy + (12 + squash)
    body_h = 11 - squash
    rect(draw, ox + 10, body_y, 13, body_h, PALETTE["body"])  # torso
    rect(draw, ox + 18, body_y - 9, 8, 9, PALETTE["body"])    # neck/head
    rect(draw, ox + 24, body_y - 6, 4, 6, PALETTE["body"])    # snout
    rect(draw, ox + 21, body_y - 6, 1, 1, PALETTE["eye"])     # eye
    rect(draw, ox + 12, body_y - 2, 8, 2, PALETTE["shade"])   # belly shade
    if crouch:
        rect(draw, ox + 7, oy + 20, 18, 7, PALETTE["body"])   # crouch body
        rect(draw, ox + 22, oy + 16, 7, 6, PALETTE["body"])   # crouch head
        rect(draw, ox + 10, oy + 27, 6, 3, PALETTE["body"])
        rect(draw, ox + 18, oy + 27, 6, 3, PALETTE["body"])


def draw_player_sheet(path):
    img = Image.new("RGBA", (128, 64), PALETTE["bg"])
    d = ImageDraw.Draw(img)

    # Frame 0 run A
    dino_base(d, 0, 0)
    rect(d, 11, 23, 4, 6, PALETTE["body"])
    rect(d, 17, 26, 4, 3, PALETTE["body"])

    # Frame 1 run B
    dino_base(d, 32, 0)
    rect(d, 43, 26, 4, 3, PALETTE["body"])
    rect(d, 49, 23, 4, 6, PALETTE["body"])

    # Frame 2 run A alt
    dino_base(d, 64, 0)
    rect(d, 75, 24, 4, 5, PALETTE["body"])
    rect(d, 81, 26, 4, 3, PALETTE["body"])

    # Frame 3 run B alt
    dino_base(d, 96, 0)
    rect(d, 107, 26, 4, 3, PALETTE["body"])
    rect(d, 113, 24, 4, 5, PALETTE["body"])

    # Frame 4 jump
    dino_base(d, 0, 32)
    rect(d, 12, 24, 4, 4, PALETTE["body"])
    rect(d, 18, 24, 4, 4, PALETTE["body"])

    # Frame 5 duck
    dino_base(d, 32, 32, crouch=True)

    # Frame 6 land squash
    dino_base(d, 64, 32, squash=2)
    rect(d, 74, 25, 6, 4, PALETTE["body"])
    rect(d, 82, 25, 6, 4, PALETTE["body"])

    # Frame 7 fall
    dino_base(d, 96, 32)
    rect(d, 108, 26, 4, 3, PALETTE["body"])
    rect(d, 114, 26, 4, 3, PALETTE["body"])

    img.save(path)


def cactus(draw, ox, variant):
    if variant == 0:
        rect(draw, ox + 14, 9, 4, 21, PALETTE["cactus"])
        rect(draw, ox + 11, 16, 3, 6, PALETTE["cactus"])
        rect(draw, ox + 18, 12, 3, 7, PALETTE["cactus"])
    elif variant == 1:
        rect(draw, ox + 13, 6, 5, 24, PALETTE["cactus"])
        rect(draw, ox + 9, 15, 4, 6, PALETTE["cactus"])
        rect(draw, ox + 18, 18, 4, 7, PALETTE["cactus"])
    else:
        rect(draw, ox + 12, 10, 8, 20, PALETTE["cactus"])
        rect(draw, ox + 9, 18, 3, 7, PALETTE["cactus"])
        rect(draw, ox + 20, 13, 3, 6, PALETTE["cactus"])
    rect(draw, ox + 14, 12, 1, 14, PALETTE["cactus_hi"])


def bird(draw, ox, wing):
    rect(draw, ox + 9, 13, 15, 6, PALETTE["bird"])
    rect(draw, ox + 24, 14, 3, 3, PALETTE["bird_dark"])
    rect(draw, ox + 12, 12, 2, 1, PALETTE["bird_dark"])
    # wings
    if wing == 0:  # up
        rect(draw, ox + 12, 8, 8, 4, PALETTE["bird"])
    elif wing == 1:  # mid
        rect(draw, ox + 12, 11, 8, 3, PALETTE["bird"])
    else:  # down
        rect(draw, ox + 12, 18, 8, 4, PALETTE["bird"])


def draw_obstacle_sheet(path):
    img = Image.new("RGBA", (192, 32), PALETTE["bg"])
    d = ImageDraw.Draw(img)
    cactus(d, 0, 0)
    cactus(d, 32, 1)
    cactus(d, 64, 2)
    bird(d, 96, 0)
    bird(d, 128, 1)
    bird(d, 160, 2)
    img.save(path)


def main():
    draw_player_sheet("assets/runtime/sprites/player.png")
    draw_obstacle_sheet("assets/runtime/sprites/obstacles.png")
    print("Generated player and obstacle sprite sheets.")


if __name__ == "__main__":
    main()
