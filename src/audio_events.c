#include "audio_events.h"

static Sound load_sound_checked(const char *path, bool *ok) {
    Sound sound = {0};

    if (!FileExists(path)) {
        TraceLog(LOG_ERROR, "Missing required sound: %s", path);
        *ok = false;
        return sound;
    }

    sound = LoadSound(path);
    if (sound.stream.buffer == 0) {
        TraceLog(LOG_ERROR, "Failed to load sound: %s", path);
        *ok = false;
    }

    return sound;
}

bool AudioEvents_Load(AudioEvents *events) {
    bool ok = true;

    if (!IsAudioDeviceReady()) {
        InitAudioDevice();
        events->owns_device = true;
    }

    events->jump = load_sound_checked("assets/runtime/sfx/jump.ogg", &ok);
    events->land = load_sound_checked("assets/runtime/sfx/land.ogg", &ok);
    events->hit = load_sound_checked("assets/runtime/sfx/hit.ogg", &ok);
    events->milestone = load_sound_checked("assets/runtime/sfx/milestone.ogg", &ok);
    events->restart = load_sound_checked("assets/runtime/sfx/restart.ogg", &ok);

    if (!ok) {
        AudioEvents_Unload(events);
        return false;
    }

    SetSoundVolume(events->jump, 0.55f);
    SetSoundVolume(events->land, 0.55f);
    SetSoundVolume(events->hit, 0.55f);
    SetSoundVolume(events->milestone, 0.55f);
    SetSoundVolume(events->restart, 0.55f);

    events->loaded = true;
    return true;
}

void AudioEvents_Play(const AudioEvents *events, AudioEventType type) {
    if (!events->loaded) {
        return;
    }

    switch (type) {
        case AUDIO_EVENT_JUMP:
            PlaySound(events->jump);
            break;
        case AUDIO_EVENT_LAND:
            PlaySound(events->land);
            break;
        case AUDIO_EVENT_HIT:
            PlaySound(events->hit);
            break;
        case AUDIO_EVENT_MILESTONE:
            PlaySound(events->milestone);
            break;
        case AUDIO_EVENT_RESTART:
            PlaySound(events->restart);
            break;
    }
}

void AudioEvents_Unload(AudioEvents *events) {
    if (events->jump.stream.buffer != 0) {
        UnloadSound(events->jump);
        events->jump = (Sound){0};
    }
    if (events->land.stream.buffer != 0) {
        UnloadSound(events->land);
        events->land = (Sound){0};
    }
    if (events->hit.stream.buffer != 0) {
        UnloadSound(events->hit);
        events->hit = (Sound){0};
    }
    if (events->milestone.stream.buffer != 0) {
        UnloadSound(events->milestone);
        events->milestone = (Sound){0};
    }
    if (events->restart.stream.buffer != 0) {
        UnloadSound(events->restart);
        events->restart = (Sound){0};
    }

    events->loaded = false;

    if (events->owns_device && IsAudioDeviceReady()) {
        CloseAudioDevice();
    }
    events->owns_device = false;
}
