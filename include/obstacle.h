#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <stdbool.h>
#include <raylib.h>

#define MAX_OBSTACLES 24

typedef enum {
    OBSTACLE_GROUND,
    OBSTACLE_AIR
} ObstacleType;

typedef struct {
    Rectangle rect;
    ObstacleType type;
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

#endif
