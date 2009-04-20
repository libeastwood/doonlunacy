#ifndef DUNE_SFX_H
#define DUNE_SFX_H

#include <boost/shared_ptr.hpp>

#include "Definitions.h"

class Mix_Chunk;
class Sound {
  public:
    Sound(Mix_Chunk *soundChunk);
    Sound() : m_soundChunk(NULL) {};
    ~Sound();

    inline Mix_Chunk *getChunk() { return m_soundChunk; }

    void concatSound(SoundPtr sound);

  private:
    Mix_Chunk *m_soundChunk;
};
#endif /* DUNE_SFX_H */
