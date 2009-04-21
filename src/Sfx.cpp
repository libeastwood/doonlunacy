#include <SDL_mixer.h>

#include <iostream>
#include "Sfx.h"

Sound::Sound(Mix_Chunk *soundChunk) : Mix_Chunk(*soundChunk) {
}

Sound::~Sound() {
    /* This is a workaround for using Mix_FreeChunk as we cannot call it on
       the object itself in it's destructor as it would attempt to free it.
       Therefore we just copy the struct with it's members to a new variable
       so that the same data will get freed along with this variable.
    */

    Mix_Chunk *chunk = (Mix_Chunk *)SDL_malloc(sizeof(*chunk));
    SDL_memcpy(chunk, this, sizeof(*chunk));

    Mix_FreeChunk(chunk);
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

	SDL_memcpy(abuf, sound->abuf, sound->alen);
    }
    else {
	alen += sound->alen;

	if((abuf = (Uint8 *)realloc(abuf, alen)) == NULL)
	    throw(std::bad_alloc());

	SDL_memcpy(abuf + (alen - sound->alen), sound->abuf, sound->alen);
    }
}

