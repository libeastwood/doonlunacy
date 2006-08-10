#ifndef DUNE_OPTIONSMENU_H
#define DUNE_OPTIONSMENU_H

#include "MenuBase.h"
#include "gui2/Button.h"
#include "gui2/VBox.h"

#include "SDL.h"


class OptionsMenuState  : public MenuBaseState 
{
    public:
        OptionsMenuState();
        ~OptionsMenuState();

        void doOptions();
        void doOk();
        void doResolution();
        void doScreenMode();

        virtual const char* GetName() { return "OptionsMenuState"; }

    private:
        VBox* m_vbox;
        
        BoringButton* m_butResolution;
        BoringButton* m_butWindowMode;
        BoringButton* m_butOk;
};

#endif // DUNE_OPTIONSMENU_H
