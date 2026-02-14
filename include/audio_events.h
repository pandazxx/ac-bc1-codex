#ifndef AUDIO_EVENTS_H
#define AUDIO_EVENTS_H

#include <stdbool.h>

#include <raylib.h>

typedef enum {
    AUDIO_EVENT_JUMP,
    AUDIO_EVENT_LAND,
    AUDIO_EVENT_HIT,
    AUDIO_EVENT_MILESTONE,
    AUDIO_EVENT_RESTART
} AudioEventType;

typedef struct {
    Sound jump;
    Sound land;
    Sound hit;
    Sound milestone;
    Sound restart;
    bool loaded;
    bool owns_device;
} AudioEvents;

bool AudioEvents_Load(AudioEvents *events);
void AudioEvents_Play(const AudioEvents *events, AudioEventType type);
void AudioEvents_Unload(AudioEvents *events);

#endif
