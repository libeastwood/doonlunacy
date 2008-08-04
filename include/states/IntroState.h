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
typedef std::pair <uint16_t, std::string> introSound;
typedef std::pair <uint16_t, Mix_Chunk*> soundChunk;
typedef std::pair <uint8_t, uint8_t> videoLoop;

class Font;
class StringFile;
class Button;
class Label;
class Container;
class IntroState : public State 
{
   
    class Frame
    {
        public:
            enum Transition
            {
                NO_TRANSITION,
                FADE_OUT
            };
            
            enum TransitionState
            {
                TRANSITION_IN,
                TRANSITION_OUT,
                PLAYING,
                HOLDING
            };
        
            Frame(std::string filename, Transition in, Transition out,
					bool cont, uint8_t endWait = 0);
			~Frame();
            
            bool Execute(float ft);
            void Load(Frame* lastframe);
			void addSound(uint16_t playAt, std::string sound);
			void concatSound(uint16_t playAt, std::string sound);
			void setSong(uint16_t song);
			void setPalette(Palette_enum palette);
			void setFps(float fps);
			void addLoop(uint8_t loopAt, uint8_t rewindTo, uint8_t numLoops, uint8_t wait);
			void addText(uint16_t playAt, std::string introText);
			void setTextColor(uint8_t textColor);
			void setTextLocation(SPoint textLocation);
			void setTextSize(float setTextSize);
			void setTextFade(bool textFade);

        private:
            std::string m_filename;
            
            bool m_continuation, m_textFade;
            float m_hold;
            
            TransitionState m_state;
            Transition m_transition_in;
            Transition m_transition_out;
            bool mb_finished;
            
            WsafilePtr m_wsa; 
            uint8_t m_currentFrame, m_framesPlayed;

            float m_frametime, m_textSize;
			int16_t m_song;
			uint8_t m_endWait, m_textColor, m_textTransition;
			uint16_t m_totalFrames;
			SPoint m_textLocation;

			float m_fps;
			videoLoop m_loop, m_loopTime;

            ImagePtr m_animSurface, m_scaledSurface, m_textSurface;
            SDL_Color* m_transitionPalette;
			Font* font;
			std::vector<videoLoop> m_loops;
			std::vector<introText> m_introStrings;
			std::vector<introSound> m_introSounds;
			std::vector<soundChunk> m_soundChunks;

			SDL_Palette* m_palette;


            void setupTransitionIn();
            void setupTransitionOut(ImagePtr img);
            void cleanupTransitionIn();
            void cleanupTransitionOut();
            
            void doTransitionIn(float dt);
            void doTransitionOut(float dt);
			void doTransitionOut(ImagePtr img, bool done = false, bool forceTransition = false, const int fadeAmt = 4);
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
