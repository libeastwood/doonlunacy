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
            typedef enum Transition
            {
                NO_TRANSITION,
                FADE_OUT
            };
            
            typedef enum TransitionState
            {
                TRANSITION_IN,
                TRANSITION_OUT,
                PLAYING,
                HOLDING
            };
        
            Frame(std::string file, Transition in, Transition out, bool cont); 
            
            bool Execute(float ft);
            void Load(Frame* lastframe);

        private:
            std::string m_filename;
            
            bool m_continuation;
            float m_hold;
            
            TransitionState m_state;
            Transition m_transition_in;
            Transition m_transition_out;
            bool mb_finished;
            
            Wsafile *m_wsa; 
            int m_currentFrame;
            float m_frametime;

            SDL_Surface* m_animSurface, *m_scaledSurface;
            SDL_Color* m_transitionPalette;

            void setupTransitionIn();
            void setupTransitionOut();
            void cleanupTransitionIn();
            void cleanupTransitionOut();
            
            void doTransitionIn(float dt);
            void doTransitionOut(float dt);
            void doPlaying(float dt);
            void doHolding(float dt);
    };
    
    
    typedef std::list<Frame*> IntroList;
    
    
    public:
        IntroState();
        ~IntroState();

        void JustMadeActive();
        void JustMadeInactive();

        int Execute(float dt);
        void SkipIntro();
        void enque(Frame* frame) { m_wsaNames.push_back(frame); }
        bool next();
        void load(Frame frame);
        virtual const char* GetName() { return "IntroState"; }

    private:

        IntroList m_wsaNames;

        TranspButton *m_butIntro;

        Frame* m_currentFrame;
};



#endif // DUNE_INTROSTATE_H
