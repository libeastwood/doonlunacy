#ifndef DUNE_SINGLEMENU_H
#define DUNE_SINGLEMENU_H

#include "MainMenuBase.h"
#include "gui2/Button.h"
#include "gui2/VBox.h"

#include "SDL.h"


class SingleMenuState : public MainMenuBaseState 
{
    public:
        SingleMenuState();
        ~SingleMenuState();

		void doCampaign();
        void doSkirmish();
        void doCancel();

        virtual const char* GetName() { return "SingleMenuState"; }

    private:
        BoringButton* m_butCampaign;
        BoringButton* m_butCustom;
        BoringButton* m_butSkirmish;
        BoringButton* m_butLoad;
        BoringButton* m_butCancel;
};

#endif // DUNE_SINGLEMENU_H
