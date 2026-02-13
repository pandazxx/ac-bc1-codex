#include "score_store.h"

#include <stdio.h>
#include <stdlib.h>

#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>

EM_JS(int, load_best_from_storage, (), {
    const raw = globalThis.localStorage ? localStorage.getItem('dino_best_score') : null;
    const value = raw === null ? 0 : Number(raw);
    return Number.isFinite(value) ? value | 0 : 0;
});

EM_JS(void, save_best_to_storage, (int score), {
    if (globalThis.localStorage) {
        localStorage.setItem('dino_best_score', String(score));
    }
});
#endif

int ScoreStore_LoadBest(void) {
#if defined(__EMSCRIPTEN__)
    return load_best_from_storage();
#else
    FILE *file = fopen("save/highscore.txt", "r");
    if (!file) {
        return 0;
    }

    int value = 0;
    int scanned = fscanf(file, "%d", &value);
    fclose(file);
    return scanned == 1 ? value : 0;
#endif
}

void ScoreStore_SaveBest(int score) {
#if defined(__EMSCRIPTEN__)
    save_best_to_storage(score);
#else
    FILE *file = fopen("save/highscore.txt", "w");
    if (!file) {
        return;
    }

    fprintf(file, "%d\n", score);
    fclose(file);
#endif
}
