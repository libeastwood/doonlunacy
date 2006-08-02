#ifndef DUNE_SINGLEMENU_H
#define DUNE_SINGLEMENU_H

#include "State.h"
#include "gui2/Button.h"
#include "gui2/VBox.h"

#include "SDL.h"


class SingleMenuState : public State 
{
    public:
        SingleMenuState();
        ~SingleMenuState();

        void JustMadeActive();
        void JustMadeInactive();

        int Execute(float dt);

        void doSkirmish();
        void doCancel();

        virtual const char* GetName() { return "SingleMenuState"; }

    private:
        SDL_Surface* m_menuBackground;
        //Window* m_menu;

        VBox* m_vbox;
        
        BoringButton* m_butCampaign;
        BoringButton* m_butCustom;
        BoringButton* m_butSkirmish;
        BoringButton* m_butLoad;
        BoringButton* m_butCancel;
};

#endif // DUNE_SINGLEMENU_H
