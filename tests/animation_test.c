#include <stdio.h>

#include "animation.h"

static int assert_eq_int(const char *label, int got, int want) {
    if (got == want) {
        return 0;
    }

    fprintf(stderr, "%s failed: got %d, want %d\n", label, got, want);
    return 1;
}

int main(void) {
    int failures = 0;

    AnimationPlayer run = {0};
    AnimationClip run_clip = {0, 4, 12.0f, true};
    Animation_SetClip(&run, run_clip, true);

    // 5 ticks at 1/60 with 12 fps -> exactly 1 frame advance.
    for (int i = 0; i < 5; i++) {
        Animation_Update(&run, 1.0f / 60.0f);
    }
    failures += assert_eq_int("run_frame_after_5_ticks", Animation_CurrentFrame(&run), 1);

    // 15 more ticks -> total 20 ticks = 4 frame times -> loop back to frame 0.
    for (int i = 0; i < 15; i++) {
        Animation_Update(&run, 1.0f / 60.0f);
    }
    failures += assert_eq_int("run_loop_at_20_ticks", Animation_CurrentFrame(&run), 0);

    AnimationPlayer landing = {0};
    AnimationClip landing_clip = {6, 2, 10.0f, false};
    Animation_SetClip(&landing, landing_clip, true);

    // 12 ticks at 1/60 = 0.2s -> should be finished at last frame.
    for (int i = 0; i < 12; i++) {
        Animation_Update(&landing, 1.0f / 60.0f);
    }
    failures += assert_eq_int("landing_last_frame", Animation_CurrentFrame(&landing), 7);

    return failures;
}
