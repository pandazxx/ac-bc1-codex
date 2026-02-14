#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdbool.h>

typedef struct {
    int start_frame;
    int frame_count;
    float fps;
    bool loop;
} AnimationClip;

typedef struct {
    AnimationClip clip;
    int frame_index;
    float timer;
    bool finished;
} AnimationPlayer;

void Animation_SetClip(AnimationPlayer *player, AnimationClip clip, bool restart);
void Animation_Update(AnimationPlayer *player, float dt);
int Animation_CurrentFrame(const AnimationPlayer *player);

#endif
