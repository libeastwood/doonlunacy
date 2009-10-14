#ifndef DUNE_SFX_H
#define DUNE_SFX_H

#include <boost/shared_ptr.hpp>
#include <eastwood/SDL/Mixer/Sound.h>

#include "Definitions.h"

class SoundPlayer;

class Sound : public eastwood::SDL::Mixer::Sound {
  public:
    Sound() : eastwood::SDL::Mixer::Sound() {};
    Sound(const eastwood::SDL::Mixer::Sound &sound) : eastwood::SDL::Mixer::Sound(sound) {};
    ~Sound();

    Sound concatSound(const Sound &sound);

    friend class SoundPlayer;

};
#endif /* DUNE_SFX_H */
