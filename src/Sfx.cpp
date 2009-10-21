#include "Sfx.h"

SoundPtr concatSound(const SoundPtr sound1, const SoundPtr sound2) {
    uint32_t newSize = sound1->size() + sound2->size();
    uint8_t *buffer = new uint8_t[newSize];
    memcpy(buffer, *sound1, sound1->size());
    memcpy(buffer+sound1->size(), *sound2, sound2->size());
    return SoundPtr(new eastwood::SDL::Mixer::Sound(newSize, buffer, sound1->frequency(), sound1->channels(), sound1->format()));
}

