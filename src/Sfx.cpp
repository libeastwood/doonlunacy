#include <SDL_mixer.h>

#include "Sfx.h"

Sound::Sound(Mix_Chunk *soundChunk) : m_soundChunk(soundChunk) {

}

Sound::~Sound() {
    if(m_soundChunk)
    	Mix_FreeChunk(m_soundChunk);
}

void Sound::concatSound(SoundPtr sound) {
    if(!m_soundChunk)
	memcpy(m_soundChunk->abuf, sound->getChunk()->abuf, sound->getChunk()->alen);
    else {
	Mix_Chunk *newChunk;
	if((newChunk = (Mix_Chunk*) malloc(sizeof(Mix_Chunk))) == NULL)
	    throw(std::bad_alloc());

	newChunk->allocated = 1;
	newChunk->volume = m_soundChunk->volume;
	newChunk->alen = m_soundChunk->alen + sound->getChunk()->alen;

	if((newChunk->abuf = (Uint8 *)malloc(newChunk->alen)) == NULL) {
	    free(newChunk);
	    throw(std::bad_alloc());
	}

	memcpy(newChunk->abuf, m_soundChunk->abuf, m_soundChunk->alen);
	memcpy(newChunk->abuf + m_soundChunk->alen, sound->getChunk()->abuf, sound->getChunk()->alen);
	Mix_FreeChunk(m_soundChunk);
	m_soundChunk = newChunk;
    }
}

