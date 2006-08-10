#ifndef DUNE_SINGLEMENU_H
#define DUNE_SINGLEMENU_H

#include "MenuBase.h"
#include "gui2/Button.h"
#include "gui2/VBox.h"

#include "SDL.h"


class SingleMenuState : public MenuBaseState 
{
    public:
        SingleMenuState();
        ~SingleMenuState();

        void doSkirmish();
        void doCancel();

        virtual const char* GetName() { return "SingleMenuState"; }

    private:
        VBox* m_vbox;
        
        BoringButton* m_butCampaign;
        BoringButton* m_butCustom;
        BoringButton* m_butSkirmish;
        BoringButton* m_butLoad;
        BoringButton* m_butCancel;
};

#endif // DUNE_SINGLEMENU_H
