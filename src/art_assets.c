#include "art_assets.h"

#include <stdio.h>

static Texture2D load_texture_checked(const char *path, bool *ok) {
    Texture2D texture = {0};
    if (!FileExists(path)) {
        TraceLog(LOG_ERROR, "Missing required texture: %s", path);
        *ok = false;
        return texture;
    }

    texture = LoadTexture(path);
    if (texture.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load texture: %s", path);
        *ok = false;
    }

    return texture;
}

bool ArtAssets_Load(ArtAssets *assets) {
    bool ok = true;

    assets->player_sheet = load_texture_checked("assets/runtime/sprites/player.png", &ok);
    assets->obstacle_sheet = load_texture_checked("assets/runtime/sprites/obstacles.png", &ok);

    if (!ok) {
        ArtAssets_Unload(assets);
        return false;
    }

    SetTextureFilter(assets->player_sheet, TEXTURE_FILTER_POINT);
    SetTextureFilter(assets->obstacle_sheet, TEXTURE_FILTER_POINT);

    assets->loaded = true;
    return true;
}

void ArtAssets_Unload(ArtAssets *assets) {
    if (assets->player_sheet.id != 0) {
        UnloadTexture(assets->player_sheet);
        assets->player_sheet = (Texture2D){0};
    }

    if (assets->obstacle_sheet.id != 0) {
        UnloadTexture(assets->obstacle_sheet);
        assets->obstacle_sheet = (Texture2D){0};
    }

    assets->loaded = false;
}
