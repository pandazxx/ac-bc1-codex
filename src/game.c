#include "game.h"

#include <math.h>

#include <raylib.h>

#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif

#include "animation.h"
#include "art_assets.h"
#include "audio_events.h"
#include "obstacle.h"
#include "player.h"
#include "score_store.h"

static const int WINDOW_WIDTH = 960;
static const int WINDOW_HEIGHT = 540;
static const int VIRTUAL_WIDTH = 480;
static const int VIRTUAL_HEIGHT = 270;

static const float GROUND_Y = 228.0f;
static const float BASE_SPEED = 300.0f;
static const float SPEED_STEP = 30.0f;
static const float MAX_SPEED = 650.0f;

static const float FIXED_DT = 1.0f / 60.0f;
static const float MAX_FRAME_DT = 0.25f;
static const int MAX_STEPS_PER_FRAME = 5;

static const float HIT_FREEZE_S = 0.050f;
static const float HIT_SHAKE_S = 0.120f;
static const float LAND_POSE_S = 0.080f;

static const AnimationClip PLAYER_RUN_CLIP = {0, 4, 12.0f, true};
static const AnimationClip BIRD_FLAP_CLIP = {3, 3, 10.0f, true};

static GameState g_game = {0};
static Player g_player;
static ObstacleManager g_obstacles;

static RenderTexture2D g_scene_target = {0};
static ArtAssets g_art = {0};
static AudioEvents g_audio = {0};

static AnimationPlayer g_player_anim = {0};
static AnimationPlayer g_bird_anim = {0};

static float g_accumulator = 0.0f;
static float g_hit_freeze_timer = 0.0f;
static float g_hit_shake_timer = 0.0f;
static float g_land_pose_timer = 0.0f;
static bool g_show_debug_hitboxes = false;
static int g_last_milestone_played = 0;

#if defined(__EMSCRIPTEN__)
EM_JS(int, web_consume_debug_toggle_count, (), {
    if (typeof Module !== 'undefined' && Module.debugToggleCount > 0) {
        var count = Module.debugToggleCount | 0;
        Module.debugToggleCount = 0;
        return count;
    }
    return 0;
});

EM_JS(int, web_consume_audio_unlock, (), {
    if (typeof Module !== 'undefined' && Module.audioUnlockRequested) {
        Module.audioUnlockRequested = false;
        return 1;
    }
    return 0;
});

EM_JS(void, web_resume_audio_context, (), {
    try {
        if (typeof Module !== 'undefined' && Module.SDL2 && Module.SDL2.audioContext) {
            Module.SDL2.audioContext.resume();
        }
        if (typeof AL !== 'undefined' && AL.currentContext && AL.currentContext.ctx) {
            AL.currentContext.ctx.resume();
        }
    } catch (e) {}
});
#endif

static void reset_run_state(void) {
    g_game.phase = GAME_RUNNING;
    g_game.score = 0;
    g_game.speed = BASE_SPEED;

    g_hit_freeze_timer = 0.0f;
    g_hit_shake_timer = 0.0f;
    g_land_pose_timer = 0.0f;
    g_last_milestone_played = 0;

    Player_Reset(&g_player, 64.0f, GROUND_Y);
    Obstacles_Reset(&g_obstacles);

    Animation_SetClip(&g_player_anim, PLAYER_RUN_CLIP, true);
    Animation_SetClip(&g_bird_anim, BIRD_FLAP_CLIP, true);
}

static bool any_key_pressed(void) {
    return GetKeyPressed() != 0;
}

static Rectangle frame_rect_from_index(int frame_index, int frames_per_row, int frame_size) {
    int col = frame_index % frames_per_row;
    int row = frame_index / frames_per_row;
    return (Rectangle){
        (float)(col * frame_size),
        (float)(row * frame_size),
        (float)frame_size,
        (float)frame_size,
    };
}

static int player_frame_index(void) {
    if (!g_player.on_ground) {
        return g_player.velocity_y < 0.0f ? 4 : 7;
    }

    if (g_player.ducking) {
        return 5;
    }

    if (g_land_pose_timer > 0.0f) {
        return 6;
    }

    return Animation_CurrentFrame(&g_player_anim);
}

static int obstacle_frame_index(const Obstacle *obstacle) {
    if (obstacle->type == OBSTACLE_AIR) {
        return Animation_CurrentFrame(&g_bird_anim);
    }

    if (obstacle->rect.width < 36.0f) {
        return 0;
    }
    if (obstacle->rect.width < 46.0f) {
        return 1;
    }
    return 2;
}

static void update_running_fixed(float dt) {
    Player_Update(&g_player, dt, GROUND_Y);

    if (g_player.jumped_this_step) {
        AudioEvents_Play(&g_audio, AUDIO_EVENT_JUMP);
    }

    if (g_player.landed_this_step) {
        AudioEvents_Play(&g_audio, AUDIO_EVENT_LAND);
        g_land_pose_timer = LAND_POSE_S;
    }

    if (g_land_pose_timer > 0.0f) {
        g_land_pose_timer -= dt;
    }

    int step = g_game.score / 100;
    g_game.speed = BASE_SPEED + (float)step * SPEED_STEP;
    if (g_game.speed > MAX_SPEED) {
        g_game.speed = MAX_SPEED;
    }

    Obstacles_Update(&g_obstacles, dt, g_game.speed, GROUND_Y, VIRTUAL_WIDTH);
    Animation_Update(&g_bird_anim, dt);

    if (g_player.on_ground && !g_player.ducking) {
        Animation_SetClip(&g_player_anim, PLAYER_RUN_CLIP, false);
        Animation_Update(&g_player_anim, dt);
    }

    if (Obstacles_CheckCollision(&g_obstacles, Player_GetHurtbox(&g_player))) {
        g_game.phase = GAME_OVER;
        g_hit_freeze_timer = HIT_FREEZE_S;
        g_hit_shake_timer = HIT_SHAKE_S;
        AudioEvents_Play(&g_audio, AUDIO_EVENT_HIT);

        if (g_game.score > g_game.best_score) {
            g_game.best_score = g_game.score;
            ScoreStore_SaveBest(g_game.best_score);
        }
        return;
    }

    g_game.score += (int)(dt * g_game.speed * 0.35f);
    int milestone = g_game.score / 100;
    if (milestone > g_last_milestone_played && milestone > 0) {
        AudioEvents_Play(&g_audio, AUDIO_EVENT_MILESTONE);
        g_last_milestone_played = milestone;
    }
}

static void update_game_over_fixed(void) {
    if (any_key_pressed()) {
        AudioEvents_Play(&g_audio, AUDIO_EVENT_RESTART);
        reset_run_state();
    }
}

static void update_fixed(float dt) {
    if (g_hit_shake_timer > 0.0f) {
        g_hit_shake_timer -= dt;
    }

    if (g_hit_freeze_timer > 0.0f) {
        g_hit_freeze_timer -= dt;
        return;
    }

    if (g_game.phase == GAME_RUNNING) {
        update_running_fixed(dt);
    } else {
        update_game_over_fixed();
    }
}

static void draw_scene(void) {
    const Color bg = (Color){245, 244, 240, 255};
    const Color fg = (Color){38, 38, 38, 255};
    const Color accent = (Color){235, 90, 40, 255};

    float shake_x = 0.0f;
    float shake_y = 0.0f;
    if (g_hit_shake_timer > 0.0f) {
        shake_x = (float)GetRandomValue(-2, 2);
        shake_y = (float)GetRandomValue(-2, 2);
    }

    BeginTextureMode(g_scene_target);
    ClearBackground(bg);

    DrawLine((int)shake_x, (int)(GROUND_Y + shake_y), VIRTUAL_WIDTH + (int)shake_x, (int)(GROUND_Y + shake_y), fg);

    if (g_art.loaded) {
        Rectangle player_src = frame_rect_from_index(player_frame_index(), 4, 32);
        Rectangle player_dst = (Rectangle){
            g_player.rect.x + shake_x,
            g_player.rect.y + shake_y,
            g_player.rect.width,
            g_player.rect.height,
        };
        DrawTexturePro(g_art.player_sheet, player_src, player_dst, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);

        const Obstacle *items = Obstacles_Items(&g_obstacles);
        size_t count = Obstacles_Count();
        for (size_t i = 0; i < count; i++) {
            const Obstacle *obstacle = &items[i];
            if (!obstacle->active) {
                continue;
            }

            int frame = obstacle_frame_index(obstacle);
            Rectangle src = frame_rect_from_index(frame, 6, 32);
            Rectangle dst = (Rectangle){
                obstacle->rect.x + shake_x,
                obstacle->rect.y + shake_y,
                obstacle->rect.width,
                obstacle->rect.height,
            };
            DrawTexturePro(g_art.obstacle_sheet, src, dst, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
        }
    } else {
        DrawRectangleRec((Rectangle){g_player.rect.x + shake_x, g_player.rect.y + shake_y, g_player.rect.width, g_player.rect.height}, accent);
        Obstacles_Draw(&g_obstacles);
    }

    if (g_show_debug_hitboxes) {
        Rectangle hurtbox = Player_GetHurtbox(&g_player);
        DrawRectangleLinesEx((Rectangle){hurtbox.x + shake_x, hurtbox.y + shake_y, hurtbox.width, hurtbox.height}, 1.0f, GREEN);

        const Obstacle *items = Obstacles_Items(&g_obstacles);
        size_t count = Obstacles_Count();
        for (size_t i = 0; i < count; i++) {
            const Obstacle *obstacle = &items[i];
            if (!obstacle->active) {
                continue;
            }
            DrawRectangleLinesEx((Rectangle){
                obstacle->rect.x + shake_x,
                obstacle->rect.y + shake_y,
                obstacle->rect.width,
                obstacle->rect.height,
            }, 1.0f, RED);
        }

        DrawText("DEBUG HITBOX: ON", VIRTUAL_WIDTH - 158, 8, 10, (Color){25, 120, 35, 255});
    }

    DrawText(TextFormat("SCORE %04d", g_game.score), 12, 12, 14, fg);
    DrawText(TextFormat("BEST %04d", g_game.best_score), 12, 28, 12, fg);

    if (g_game.phase == GAME_OVER) {
        DrawText("GAME OVER", VIRTUAL_WIDTH / 2 - 55, VIRTUAL_HEIGHT / 2 - 18, 24, fg);
        DrawText("PRESS ANY KEY", VIRTUAL_WIDTH / 2 - 55, VIRTUAL_HEIGHT / 2 + 10, 12, fg);
    }

    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);

    float scale = floorf(fminf((float)GetScreenWidth() / (float)VIRTUAL_WIDTH, (float)GetScreenHeight() / (float)VIRTUAL_HEIGHT));
    if (scale < 1.0f) {
        scale = 1.0f;
    }

    float draw_width = (float)VIRTUAL_WIDTH * scale;
    float draw_height = (float)VIRTUAL_HEIGHT * scale;
    float offset_x = ((float)GetScreenWidth() - draw_width) * 0.5f;
    float offset_y = ((float)GetScreenHeight() - draw_height) * 0.5f;

    Rectangle src = {0.0f, 0.0f, (float)VIRTUAL_WIDTH, -(float)VIRTUAL_HEIGHT};
    Rectangle dst = {offset_x, offset_y, draw_width, draw_height};
    DrawTexturePro(g_scene_target.texture, src, dst, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);

    EndDrawing();
}

void Game_Init(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Raylib Dino Runner");
    SetTargetFPS(60);

    g_scene_target = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    SetTextureFilter(g_scene_target.texture, TEXTURE_FILTER_POINT);

    ArtAssets_Load(&g_art);
    AudioEvents_Load(&g_audio);

    g_game.best_score = ScoreStore_LoadBest();

    Player_Init(&g_player, 64.0f, GROUND_Y);
    Obstacles_Init(&g_obstacles);
    reset_run_state();
}

void Game_RunFrame(void) {
    int debug_toggle_count = (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_F3)) ? 1 : 0;
#if defined(__EMSCRIPTEN__)
    debug_toggle_count += web_consume_debug_toggle_count();
    if (web_consume_audio_unlock()) {
        web_resume_audio_context();
    }
#endif
    if ((debug_toggle_count % 2) != 0) {
        g_show_debug_hitboxes = !g_show_debug_hitboxes;
    }

    float frame_dt = GetFrameTime();
    if (frame_dt > MAX_FRAME_DT) {
        frame_dt = MAX_FRAME_DT;
    }

    g_accumulator += frame_dt;

    int steps = 0;
    while (g_accumulator >= FIXED_DT && steps < MAX_STEPS_PER_FRAME) {
        update_fixed(FIXED_DT);
        g_accumulator -= FIXED_DT;
        steps++;
    }

    if (steps == MAX_STEPS_PER_FRAME && g_accumulator > FIXED_DT) {
        g_accumulator = FIXED_DT;
    }

    draw_scene();
}

bool Game_ShouldExit(void) {
    return WindowShouldClose();
}

void Game_Shutdown(void) {
    AudioEvents_Unload(&g_audio);
    ArtAssets_Unload(&g_art);

    if (g_scene_target.id != 0) {
        UnloadRenderTexture(g_scene_target);
        g_scene_target = (RenderTexture2D){0};
    }

    CloseWindow();
}
