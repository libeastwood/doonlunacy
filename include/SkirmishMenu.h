#ifndef DUNE_SKIRMISHMENU_H
#define DUNE_SKIRMISHMENU_H

#include "MainMenuBase.h"
#include "gui2/Button.h"
#include "gui2/VBox.h"

#include "SDL.h"


class SkirmishMenuState : public MainMenuBaseState 
{
    public:
        SkirmishMenuState();
        ~SkirmishMenuState();

        void doCancel();

        virtual const char* GetName() { return "SkirmishMenuState"; }

    private:
        BoringButton* m_butCancel;
};

#endif // DUNE_SINGLEMENU_H
