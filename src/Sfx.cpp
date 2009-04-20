#include <SDL_mixer.h>

#include <iostream>
#include "Sfx.h"

Sound::Sound(Mix_Chunk *soundChunk) : Mix_Chunk(*soundChunk) {
}

Sound::~Sound() {
    if(allocated)
	free(abuf);
}

void Sound::concatSound(SoundPtr sound) {
    if(!allocated)
    {
	allocated = 1;
	volume = sound->volume;
	alen = sound->alen;

	if((abuf = (Uint8 *)malloc(alen)) == NULL) {
	    throw(std::bad_alloc());
	}

	memcpy(abuf, sound->abuf, sound->alen);
    }
    else {
	alen += sound->alen;

	if((abuf = (Uint8 *)realloc(abuf, alen)) == NULL)
	    throw(std::bad_alloc());

	memcpy(abuf + (alen - sound->alen), sound->abuf, sound->alen);
    }
}

