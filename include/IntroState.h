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
typedef std::pair <uint16_t, Sound_enum> introSound;

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
        
            Frame(std::string filename, Transition in, Transition out,
					bool cont, uint16_t endWait = 0);
			~Frame();
            
            bool Execute(float ft);
            void Load(Frame* lastframe);
			void addText(uint16_t playAt, std::string introText);
			void addSound(uint16_t playAt, Sound_enum sound);
			void setPalette(Palette_enum palette);
			void setSong(uint8_t song);

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
			int8_t m_song;
			uint16_t m_endWait;


            ImagePtr m_animSurface, m_scaledSurface;
            SDL_Color* m_transitionPalette;
			Label* m_subText;
			Container* m_container;
			std::vector<introText> m_introStrings;
			std::vector<introSound> m_introSounds;

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
		void handleInput();
        IntroList m_wsaNames;

        TranspButton *m_butIntro;

        Frame* m_currentFrame;
		Frame* frame;
		std::vector<introText> m_introStrings;
		std::vector<introSound> m_introSounds;
		
//		Font* font;
};



#endif // DUNE_INTROSTATE_H
