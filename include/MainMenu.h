#ifndef DUNE_MAINMENU_H
#define DUNE_MAINMENU_H

#include "State.h"
#include "gui2/Button.h"
#include "gui2/VBox.h"

#include "SDL.h"


class MainMenuState : public State 
{
    public:
        MainMenuState();
        ~MainMenuState();

        void JustMadeActive();
        void JustMadeInactive();

        int Execute(float dt);

        void doSkirmish();
        void doSingle();
        void doQuit();
        
        virtual const char* GetName() { return "MainMenuState"; }

    private:
        SDL_Surface* m_menuBackground;
        //Window* m_menu;

        VBox* m_vbox;
        
        GraphicButton* m_butSingle;
        GraphicButton* m_butMulti;
        GraphicButton* m_butMapEd;
        GraphicButton* m_butOptions;
        GraphicButton* m_butAbout;
        GraphicButton* m_butQuit;
        
};

#endif // DUNE_MAINMENU_H
