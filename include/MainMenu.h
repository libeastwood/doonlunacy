#ifndef DUNE_MAINMENU_H
#define DUNE_MAINMENU_H

#include "MenuBase.h"
#include "gui2/Button.h"
#include "gui2/VBox.h"
#include "Gfx.h"
#include "SDL.h"

class MainMenuState : public MenuBaseState 
{
    public:
        MainMenuState();
        ~MainMenuState();

        void doOptions();
        void doSkirmish();
        void doSingle();
        void doQuit();
        int Execute(float dt);
        virtual const char* GetName() { return "MainMenuState"; }

    private:
        VBox* m_vbox;
        Rect m_rect;
        SDL_Surface * m_surf;
        BoringButton* m_butSingle;
        BoringButton* m_butMulti;
        BoringButton* m_butMapEd;
        BoringButton* m_butOptions;
        BoringButton* m_butAbout;
        BoringButton* m_butQuit;
        
};

#endif // DUNE_MAINMENU_H
