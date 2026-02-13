#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <raylib.h>

typedef struct {
    Rectangle rect;
    float velocity_y;
    bool on_ground;
    bool ducking;
    float jump_buffer_timer;
    float coyote_timer;
} Player;

void Player_Init(Player *player, float x, float ground_y);
void Player_Reset(Player *player, float x, float ground_y);
void Player_Update(Player *player, float dt, float ground_y);
Rectangle Player_GetHurtbox(const Player *player);

#endif
