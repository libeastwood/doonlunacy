#include <iostream>
#include "Sfx.h"

Sound::~Sound() {
}

Sound Sound::concatSound(const Sound &sound) {
    uint32_t newSize = _size + sound._size;
    uint8_t *buffer = new uint8_t[newSize];
    memcpy(buffer, *_buffer.get(), _size);
    memcpy(buffer+_size, *sound._buffer.get(), sound._size);
    return eastwood::SDL::Mixer::Sound(newSize, buffer, sound._frequency, sound._channels, sound._format);
}

