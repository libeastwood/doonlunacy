#ifndef DUNE_STATES_MAINMENU_H
#define DUNE_STATES_MAINMENU_H

#include "MainMenuBase.h"

class BoringButton;
class MainMenuState : public MainMenuBaseState 
{
    public:
        MainMenuState();

        void doEditor();
        void doOptions();
        void doSkirmish();
        void doSingle();
        void doQuit();
        virtual const char* GetName() { return "MainMenuState"; }

    private:
        BoringButton* m_butSingle;
        BoringButton* m_butMulti;
        BoringButton* m_butMapEd;
        BoringButton* m_butOptions;
        BoringButton* m_butAbout;
        BoringButton* m_butQuit;
        
};

#endif // DUNE_STATES_MAINMENU_H
