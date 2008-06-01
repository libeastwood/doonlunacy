#ifndef DUNE_OPTIONSMENU_H
#define DUNE_OPTIONSMENU_H

#include "MenuBase.h"

#include "gui2/Button.h"
#include "gui2/VBox.h"


class OptionsMenuState  : public MenuBaseState 
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
        
        virtual const char* GetName() { return "OptionsMenuState"; }

    private:
        VBox* m_vbox;
        BoringButton* m_butResolution;
        BoringButton* m_butWindowMode;
        BoringButton* m_butOk;
        BoringButton* m_butSound;
        BoringButton* m_butMusic;
};

#endif // DUNE_OPTIONSMENU_H
