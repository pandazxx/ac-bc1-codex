#include "obstacle.h"

#include <stddef.h>

#include <raylib.h>

static float random_range(float min, float max) {
    return min + ((float)GetRandomValue(0, 10000) / 10000.0f) * (max - min);
}

void Obstacles_Init(ObstacleManager *manager) {
    Obstacles_Reset(manager);
}

void Obstacles_Reset(ObstacleManager *manager) {
    manager->spawn_timer = 0.65f;
    for (size_t i = 0; i < MAX_OBSTACLES; i++) {
        manager->items[i].active = false;
    }
}

static void spawn_obstacle(ObstacleManager *manager, float speed, float ground_y, int screen_width) {
    for (size_t i = 0; i < MAX_OBSTACLES; i++) {
        if (manager->items[i].active) {
            continue;
        }

        bool use_air = GetRandomValue(0, 100) < 25;
        float width = use_air ? 40.0f : (float)GetRandomValue(28, 56);
        float height = use_air ? 24.0f : (float)GetRandomValue(42, 58);
        float y = use_air ? (ground_y - 92.0f) : (ground_y - height);

        manager->items[i].rect = (Rectangle){(float)screen_width + 24.0f, y, width, height};
        manager->items[i].type = use_air ? OBSTACLE_AIR : OBSTACLE_GROUND;
        manager->items[i].active = true;

        float min_gap = 200.0f + speed * 0.20f;
        float max_gap = 340.0f + speed * 0.30f;
        manager->spawn_timer = random_range(min_gap, max_gap) / speed;
        break;
    }
}

void Obstacles_Update(ObstacleManager *manager, float dt, float speed, float ground_y, int screen_width) {
    manager->spawn_timer -= dt;
    if (manager->spawn_timer <= 0.0f) {
        spawn_obstacle(manager, speed, ground_y, screen_width);
    }

    for (size_t i = 0; i < MAX_OBSTACLES; i++) {
        Obstacle *obstacle = &manager->items[i];
        if (!obstacle->active) {
            continue;
        }

        obstacle->rect.x -= speed * dt;
        if (obstacle->rect.x + obstacle->rect.width < 0.0f) {
            obstacle->active = false;
        }
    }
}

bool Obstacles_CheckCollision(const ObstacleManager *manager, Rectangle player_hurtbox) {
    const float shrink_w = 0.82f;
    const float shrink_h = 0.82f;

    for (size_t i = 0; i < MAX_OBSTACLES; i++) {
        const Obstacle *obstacle = &manager->items[i];
        if (!obstacle->active) {
            continue;
        }

        float w = obstacle->rect.width * shrink_w;
        float h = obstacle->rect.height * shrink_h;
        float x = obstacle->rect.x + (obstacle->rect.width - w) * 0.5f;
        float y = obstacle->rect.y + (obstacle->rect.height - h) * 0.5f;
        Rectangle obstacle_hurtbox = (Rectangle){x, y, w, h};

        if (CheckCollisionRecs(player_hurtbox, obstacle_hurtbox)) {
            return true;
        }
    }

    return false;
}

void Obstacles_Draw(const ObstacleManager *manager) {
    Color cactus = (Color){30, 30, 30, 255};
    Color bird = (Color){235, 90, 40, 255};

    for (size_t i = 0; i < MAX_OBSTACLES; i++) {
        const Obstacle *obstacle = &manager->items[i];
        if (!obstacle->active) {
            continue;
        }

        DrawRectangleRec(obstacle->rect, obstacle->type == OBSTACLE_GROUND ? cactus : bird);
    }
}

const Obstacle *Obstacles_Items(const ObstacleManager *manager) {
    return manager->items;
}

size_t Obstacles_Count(void) {
    return MAX_OBSTACLES;
}
