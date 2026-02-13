#include "game.h"

#include <raylib.h>

#include "obstacle.h"
#include "player.h"
#include "score_store.h"

static const int SCREEN_WIDTH = 960;
static const int SCREEN_HEIGHT = 360;
static const float GROUND_Y = 304.0f;
static const float BASE_SPEED = 300.0f;
static const float SPEED_STEP = 30.0f;
static const float MAX_SPEED = 650.0f;

static GameState g_game = {0};
static Player g_player;
static ObstacleManager g_obstacles;

static void reset_run_state(void) {
    g_game.phase = GAME_RUNNING;
    g_game.score = 0;
    g_game.speed = BASE_SPEED;
    Player_Reset(&g_player, 120.0f, GROUND_Y);
    Obstacles_Reset(&g_obstacles);
}

void Game_Init(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Dino Runner");
    SetTargetFPS(60);

    g_game.best_score = ScoreStore_LoadBest();

    Player_Init(&g_player, 120.0f, GROUND_Y);
    Obstacles_Init(&g_obstacles);
    reset_run_state();
}

static bool any_key_pressed(void) {
    return GetKeyPressed() != 0;
}

static void update_running(float dt) {
    Player_Update(&g_player, dt, GROUND_Y);

    int step = g_game.score / 100;
    g_game.speed = BASE_SPEED + (float)step * SPEED_STEP;
    if (g_game.speed > MAX_SPEED) {
        g_game.speed = MAX_SPEED;
    }

    Obstacles_Update(&g_obstacles, dt, g_game.speed, GROUND_Y, SCREEN_WIDTH);

    if (Obstacles_CheckCollision(&g_obstacles, Player_GetHurtbox(&g_player))) {
        g_game.phase = GAME_OVER;
        if (g_game.score > g_game.best_score) {
            g_game.best_score = g_game.score;
            ScoreStore_SaveBest(g_game.best_score);
        }
        return;
    }

    g_game.score += (int)(dt * g_game.speed * 0.35f);
}

static void update_game_over(void) {
    if (any_key_pressed()) {
        reset_run_state();
    }
}

static void draw_scene(void) {
    const Color bg = (Color){245, 244, 240, 255};
    const Color fg = (Color){38, 38, 38, 255};
    const Color accent = (Color){235, 90, 40, 255};

    BeginDrawing();
    ClearBackground(bg);

    DrawLine(0, (int)GROUND_Y, SCREEN_WIDTH, (int)GROUND_Y, fg);

    Rectangle player_color_rect = g_player.rect;
    DrawRectangleRec(player_color_rect, accent);

    Obstacles_Draw(&g_obstacles);

    DrawText(TextFormat("SCORE %04d", g_game.score), 24, 24, 24, fg);
    DrawText(TextFormat("BEST %04d", g_game.best_score), 24, 52, 20, fg);

    if (g_game.phase == GAME_OVER) {
        DrawText("GAME OVER", SCREEN_WIDTH / 2 - 95, SCREEN_HEIGHT / 2 - 24, 40, fg);
        DrawText("PRESS ANY KEY TO RESTART", SCREEN_WIDTH / 2 - 165, SCREEN_HEIGHT / 2 + 20, 20, fg);
    }

    EndDrawing();
}

void Game_RunFrame(void) {
    float dt = GetFrameTime();

    if (g_game.phase == GAME_RUNNING) {
        update_running(dt);
    } else {
        update_game_over();
    }

    draw_scene();
}

bool Game_ShouldExit(void) {
    return WindowShouldClose();
}

void Game_Shutdown(void) {
    CloseWindow();
}
