#ifndef DUNE_SFX_H
#define DUNE_SFX_H

#include <boost/shared_ptr.hpp>
#include <SDL_mixer.h>

#include "Definitions.h"

class SoundPlayer;

class Sound : private Mix_Chunk {
  public:
    Sound(Mix_Chunk *soundChunk);
    Sound() { allocated = 0; abuf = NULL, alen = 0, volume = 0;}
    ~Sound();

    void concatSound(SoundPtr sound);

    friend class SoundPlayer;

};
#endif /* DUNE_SFX_H */
