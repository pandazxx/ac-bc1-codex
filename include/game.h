#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

typedef enum {
    GAME_RUNNING,
    GAME_OVER
} GamePhase;

typedef struct {
    GamePhase phase;
    int score;
    int best_score;
    float speed;
} GameState;

void Game_Init(void);
void Game_RunFrame(void);
bool Game_ShouldExit(void);
void Game_Shutdown(void);

#endif
