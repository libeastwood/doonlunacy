#ifndef DUNE_OPTIONSMENU_H
#define DUNE_OPTIONSMENU_H

#include "MainMenuBase.h"

#include "gui2/Button.h"
#include "gui2/VBox.h"
#include "Definitions.h"
#include "SoundPlayer.h"

class OptionsMenuState  : public MainMenuBaseState 
{
    public:
        OptionsMenuState();
        ~OptionsMenuState();

        void doMusic();
        void doOptions();
        void doOk();
        void doResolution();
        void doScreenMode();
        void doSound();
	void doIntro();
	void doEmuOpl();	
        virtual const char* GetName() { return "OptionsMenuState"; }

    private:
        BoringButton* m_butResolution;
        BoringButton* m_butWindowMode;
        BoringButton* m_butOk;
        BoringButton* m_butSound;
        BoringButton* m_butMusic;
	BoringButton* m_butIntro;
	BoringButton* m_butEmuOpl;
};

#endif // DUNE_OPTIONSMENU_H
