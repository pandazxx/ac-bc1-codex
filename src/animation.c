#include "animation.h"

void Animation_SetClip(AnimationPlayer *player, AnimationClip clip, bool restart) {
    if (!restart && player->clip.start_frame == clip.start_frame &&
        player->clip.frame_count == clip.frame_count &&
        player->clip.fps == clip.fps &&
        player->clip.loop == clip.loop) {
        return;
    }

    player->clip = clip;
    player->frame_index = 0;
    player->timer = 0.0f;
    player->finished = false;
}

void Animation_Update(AnimationPlayer *player, float dt) {
    if (player->clip.frame_count <= 1 || player->clip.fps <= 0.0f || player->finished) {
        return;
    }

    float frame_time = 1.0f / player->clip.fps;
    player->timer += dt;

    while (player->timer >= frame_time) {
        player->timer -= frame_time;
        player->frame_index++;

        if (player->frame_index >= player->clip.frame_count) {
            if (player->clip.loop) {
                player->frame_index = 0;
            } else {
                player->frame_index = player->clip.frame_count - 1;
                player->finished = true;
                break;
            }
        }
    }
}

int Animation_CurrentFrame(const AnimationPlayer *player) {
    return player->clip.start_frame + player->frame_index;
}
