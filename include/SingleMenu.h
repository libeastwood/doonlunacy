#ifndef DUNE_SINGLEMENU_H
#define DUNE_SINGLEMENU_H

#include "MainMenuBase.h"


class BoringButton;
class SingleMenuState : public MainMenuBaseState 
{
    public:
        SingleMenuState();

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
