#include "player.h"

#include <raylib.h>

static const float PLAYER_WIDTH = 44.0f;
static const float PLAYER_HEIGHT = 52.0f;
static const float DUCK_HEIGHT = 30.0f;
static const float GRAVITY = 2150.0f;
static const float FAST_FALL_GRAVITY = 2900.0f;
static const float JUMP_VELOCITY = 730.0f;
static const float JUMP_BUFFER_S = 0.075f;
static const float COYOTE_S = 0.050f;

void Player_Init(Player *player, float x, float ground_y) {
    player->rect = (Rectangle){x, ground_y - PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT};
    player->velocity_y = 0.0f;
    player->on_ground = true;
    player->ducking = false;
    player->jumped_this_step = false;
    player->landed_this_step = false;
    player->jump_buffer_timer = 0.0f;
    player->coyote_timer = COYOTE_S;
}

void Player_Reset(Player *player, float x, float ground_y) {
    Player_Init(player, x, ground_y);
}

void Player_Update(Player *player, float dt, float ground_y) {
    player->jumped_this_step = false;
    player->landed_this_step = false;

    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)) {
        player->jump_buffer_timer = JUMP_BUFFER_S;
    }

    if (player->jump_buffer_timer > 0.0f) {
        player->jump_buffer_timer -= dt;
    }

    if (player->on_ground) {
        player->coyote_timer = COYOTE_S;
    } else if (player->coyote_timer > 0.0f) {
        player->coyote_timer -= dt;
    }

    player->ducking = player->on_ground && IsKeyDown(KEY_DOWN);

    float target_height = player->ducking ? DUCK_HEIGHT : PLAYER_HEIGHT;
    player->rect.height = target_height;

    if (player->jump_buffer_timer > 0.0f && player->coyote_timer > 0.0f) {
        player->velocity_y = -JUMP_VELOCITY;
        player->on_ground = false;
        player->jumped_this_step = true;
        player->jump_buffer_timer = 0.0f;
        player->coyote_timer = 0.0f;
    }

    float gravity = player->velocity_y > 0.0f ? FAST_FALL_GRAVITY : GRAVITY;
    player->velocity_y += gravity * dt;
    player->rect.y += player->velocity_y * dt;

    float floor_y = ground_y - player->rect.height;
    bool was_on_ground = player->on_ground;
    if (player->rect.y >= floor_y) {
        player->rect.y = floor_y;
        player->velocity_y = 0.0f;
        player->on_ground = true;
        if (!was_on_ground) {
            player->landed_this_step = true;
        }
    } else {
        player->on_ground = false;
    }
}

Rectangle Player_GetHurtbox(const Player *player) {
    const float shrink_w = 0.68f;
    const float shrink_h = 0.72f;
    float w = player->rect.width * shrink_w;
    float h = player->rect.height * shrink_h;
    float x = player->rect.x + (player->rect.width - w) * 0.5f;
    float y = player->rect.y + (player->rect.height - h) * 0.5f;
    return (Rectangle){x, y, w, h};
}
