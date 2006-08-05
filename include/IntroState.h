#ifndef DUNE_INTROSTATE_H
#define DUNE_INTROSTATE_H

#include "State.h"
#include "gui2/Button.h"
#include "pakfile/Wsafile.h"

#include "SDL.h"
#include <list>
#include <string>

class IntroState : public State 
{
    class Frame
    {
        public:
            Frame(std::string file, bool cont, float h) : filename(file), 
                                                          continuation(cont), 
                                                          hold(h) {};
            std::string filename;
            bool continuation;
            float hold;  // not implemented yet
    };
    
    typedef std::list<Frame> IntroList;
    
    
    public:
        IntroState();
        ~IntroState();

        void JustMadeActive();
        void JustMadeInactive();

        int Execute(float dt);
        void SkipIntro();
        void enque(Frame frame) { m_wsaNames.push_back(frame); }
        bool next();
        void load(Frame frame);
        virtual const char* GetName() { return "IntroState"; }

    public:
        SDL_Surface* m_animSurface;

        IntroList m_wsaNames;
        
        Wsafile *m_wsa; 

        TranspButton *m_butIntro;
        int m_currentFrame;
        float m_frametime;
};

#endif // DUNE_INTROSTATE_H
