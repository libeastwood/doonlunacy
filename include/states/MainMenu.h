#ifndef DUNE_MAINMENU_H
#define DUNE_MAINMENU_H

#include "MainMenuBase.h"
#include "gui2/Button.h"
#include "gui2/VBox.h"
#include "Gfx.h"
#include "SDL.h"

class MainMenuState : public MainMenuBaseState 
{
    public:
        MainMenuState();
        ~MainMenuState();

        void doOptions();
        void doSkirmish();
        void doSingle();
        void doQuit();
        virtual const char* GetName() { return "MainMenuState"; }

    private:
        BoringButton* m_butSingle;
        BoringButton* m_butMulti;
        BoringButton* m_butOptions;
        BoringButton* m_butAbout;
        BoringButton* m_butQuit;
        
};

#endif // DUNE_MAINMENU_H
