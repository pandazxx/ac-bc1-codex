#include "obstacle.h"

#include <math.h>
#include <stddef.h>

#include <raylib.h>

static float random_range(float min, float max) {
    return min + ((float)GetRandomValue(0, 10000) / 10000.0f) * (max - min);
}

static const float TAU = 6.28318530718f;
static const float AIR_WAVE_HZ = 1.6f;
static const float AIR_WAVE_ANGULAR_VELOCITY = AIR_WAVE_HZ * TAU;
static const float AIR_COLLISION_CLEARANCE_PX = 1.0f;

Rectangle Obstacle_GetHurtbox(const Obstacle *obstacle) {
    const float shrink_w = 0.82f;
    const float shrink_h = 0.82f;

    float w = obstacle->rect.width * shrink_w;
    float h = obstacle->rect.height * shrink_h;
    float x = obstacle->rect.x + (obstacle->rect.width - w) * 0.5f;
    float y = obstacle->rect.y + (obstacle->rect.height - h) * 0.5f;
    return (Rectangle){x, y, w, h};
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
        float height = use_air ? 22.0f : (float)GetRandomValue(42, 58);
        float y = ground_y - height;

        float base_y = y;
        float wave_phase = 0.0f;
        float wave_amplitude = 0.0f;
        if (use_air) {
            // Large sweep experiment:
            // - lowest point is computed from collision boxes so ducking is safe
            // - highest point reaches jump-apex height (jumping becomes difficult)
            const float player_duck_height = 30.0f;
            const float player_duck_hurtbox_shrink_h = 0.72f;
            const float obstacle_hurtbox_shrink_h = 0.82f;

            // Duck hurtbox top (player hurtbox is foot-anchored in Player_GetHurtbox).
            const float duck_player_hurtbox_top_y = ground_y - (player_duck_height * player_duck_hurtbox_shrink_h);
            // For centered obstacle hurtbox, bottom offset from sprite top = h * (1 + shrink_h) / 2.
            const float obstacle_hurtbox_bottom_offset = height * (1.0f + obstacle_hurtbox_shrink_h) * 0.5f;
            // Lowest sprite top that keeps obstacle hurtbox bottom above duck hurtbox top by 1px.
            const float air_lowest_top_y =
                duck_player_hurtbox_top_y - AIR_COLLISION_CLEARANCE_PX - obstacle_hurtbox_bottom_offset;
            const float air_highest_top_y = ground_y - 178.0f;
            base_y = (air_lowest_top_y + air_highest_top_y) * 0.5f;
            wave_phase = random_range(0.0f, TAU);
            wave_amplitude = (air_lowest_top_y - air_highest_top_y) * 0.5f;
            y = base_y + sinf(wave_phase) * wave_amplitude;
        }

        manager->items[i].rect = (Rectangle){(float)screen_width + 24.0f, y, width, height};
        manager->items[i].type = use_air ? OBSTACLE_AIR : OBSTACLE_GROUND;
        manager->items[i].base_y = base_y;
        manager->items[i].wave_phase = wave_phase;
        manager->items[i].wave_amplitude = wave_amplitude;
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
        if (obstacle->type == OBSTACLE_AIR) {
            obstacle->wave_phase += AIR_WAVE_ANGULAR_VELOCITY * dt;
            if (obstacle->wave_phase > TAU) {
                obstacle->wave_phase -= TAU;
            }
            obstacle->rect.y = obstacle->base_y + sinf(obstacle->wave_phase) * obstacle->wave_amplitude;
        }
        if (obstacle->rect.x + obstacle->rect.width < 0.0f) {
            obstacle->active = false;
        }
    }
}

bool Obstacles_CheckCollision(const ObstacleManager *manager, Rectangle player_hurtbox) {
    for (size_t i = 0; i < MAX_OBSTACLES; i++) {
        const Obstacle *obstacle = &manager->items[i];
        if (!obstacle->active) {
            continue;
        }

        if (CheckCollisionRecs(player_hurtbox, Obstacle_GetHurtbox(obstacle))) {
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
