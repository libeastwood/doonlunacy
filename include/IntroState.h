#ifndef DUNE_INTROSTATE_H
#define DUNE_INTROSTATE_H

#include "State.h"
#include "pakfile/Wsafile.h"

#include "SDL.h"
#include <list>
#include <string>

class IntroState : public State 
{
    typedef std::list<std::string> IntroList;
    
    public:
        IntroState();
        ~IntroState();

        void JustMadeActive();
        void JustMadeInactive();

        int Execute(float dt);

        void enque(std::string file) { m_wsaNames.push_back(file); }
        bool next();
        void load(std::string file);

    public:
        SDL_Surface* m_animSurface;

        IntroList m_wsaNames;
        
        Wsafile *m_wsa; 

        int m_currentFrame;
        float m_frametime;
};

#endif // DUNE_INTROSTATE_H