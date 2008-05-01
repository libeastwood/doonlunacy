#ifndef DUNE_INTROSTATE_H
#define DUNE_INTROSTATE_H

#include "Gfx.h"
#include "State.h"

#include "gui2/Button.h"
#include "pakfile/Wsafile.h"
#include "DataCache.h"

#include "SDL.h"

#include <list>
#include <string>

typedef std::pair <uint16_t, std::string> introText;

class StringFile;
class Button;
class Label;
class Container;
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
        
            Frame(std::string file, Transition in, Transition out, std::vector<introText> introStrings, bool cont, int8_t song = -1, Palette_enum pal = INTRO_PAL);
			~Frame();
            
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
            
            WsafilePtr m_wsa; 
            int m_currentFrame;
            float m_frametime;
			int m_song;

            ImagePtr m_animSurface, m_scaledSurface;
            SDL_Color* m_transitionPalette;
			Label* m_subText;
			Container* m_container;
			std::vector<introText> m_introStrings;
			SDL_Palette* m_palette;


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
		StringFile* m_introStringFile;

    private:
        IntroList m_wsaNames;

        TranspButton *m_butIntro;

        Frame* m_currentFrame;
		std::vector<introText> m_introStrings;
		
//		Font* font;
};



#endif // DUNE_INTROSTATE_H
