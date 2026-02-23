#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <stdbool.h>
#include <stddef.h>
#include <raylib.h>

#define MAX_OBSTACLES 24

typedef enum {
    OBSTACLE_GROUND,
    OBSTACLE_AIR
} ObstacleType;

typedef struct {
    Rectangle rect;
    ObstacleType type;
    float base_y;
    float wave_phase;
    float wave_amplitude;
    bool active;
} Obstacle;

typedef struct {
    Obstacle items[MAX_OBSTACLES];
    float spawn_timer;
} ObstacleManager;

void Obstacles_Init(ObstacleManager *manager);
void Obstacles_Reset(ObstacleManager *manager);
void Obstacles_Update(ObstacleManager *manager, float dt, float speed, float ground_y, int screen_width);
bool Obstacles_CheckCollision(const ObstacleManager *manager, Rectangle player_hurtbox);
void Obstacles_Draw(const ObstacleManager *manager);
const Obstacle *Obstacles_Items(const ObstacleManager *manager);
size_t Obstacles_Count(void);
Rectangle Obstacle_GetHurtbox(const Obstacle *obstacle);

#endif
