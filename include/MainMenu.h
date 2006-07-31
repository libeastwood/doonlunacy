#ifndef DUNE_MAINMENU_H
#define DUNE_MAINMENU_H

#include "MenuBase.h"
#include "gui2/Button.h"
#include "gui2/VBox.h"

class MainMenuState : public MenuBaseState 
{
    public:
        MainMenuState();
        ~MainMenuState();

        //void JustMadeActive();
        //void JustMadeInactive();

        //int Execute(float dt);

        void doSkirmish();
        void doSingle();
        void doQuit();
        
        virtual const char* GetName() { return "MainMenuState"; }

    private:
        VBox* m_vbox;
        
        //GraphicButton* m_butSingle;
        //GraphicButton* m_butMulti;
        //GraphicButton* m_butMapEd;
        //GraphicButton* m_butOptions;
        //GraphicButton* m_butAbout;
        //GraphicButton* m_butQuit;

        BoringButton* m_butSingle;
        BoringButton* m_butMulti;
        BoringButton* m_butMapEd;
        BoringButton* m_butOptions;
        BoringButton* m_butAbout;
        BoringButton* m_butQuit;
        
};

#endif // DUNE_MAINMENU_H
