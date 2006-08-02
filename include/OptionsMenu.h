#ifndef DUNE_OPTIONSMENU_H
#define DUNE_OPTIONSMENU_H

#include "State.h"
#include "gui2/Button.h"
#include "gui2/VBox.h"

#include "SDL.h"


class OptionsMenuState : public State 
{
    public:
        OptionsMenuState();
        ~OptionsMenuState();

        void JustMadeActive();
        void JustMadeInactive();

        int Execute(float dt);
        void doOptions();
        void doOk();
        void doResolution();
        void doScreenMode();

        virtual const char* GetName() { return "OptionsMenuState"; }

    private:
        SDL_Surface* m_menuBackground;
        //Window* m_menu;

        VBox* m_vbox;
        
        BoringButton* m_butResolution;
        BoringButton* m_butWindowMode;
        BoringButton* m_butOk;
};

#endif // DUNE_OPTIONSMENU_H
