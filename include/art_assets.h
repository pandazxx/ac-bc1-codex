#ifndef ART_ASSETS_H
#define ART_ASSETS_H

#include <stdbool.h>

#include <raylib.h>

typedef struct {
    Texture2D player_sheet;
    Texture2D obstacle_sheet;
    bool loaded;
} ArtAssets;

bool ArtAssets_Load(ArtAssets *assets);
void ArtAssets_Unload(ArtAssets *assets);

#endif
