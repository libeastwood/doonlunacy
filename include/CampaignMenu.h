#ifndef DUNE_CAMPAIGNMENU_H
#define DUNE_CAMPAIGNMENU_H

#include "MenuBase.h"
#include "gui2/Button.h"
#include "gui2/VBox.h"

#include "SDL.h"


class CampaignMenuState : public MenuBaseState 
{
    public:
        CampaignMenuState();
        ~CampaignMenuState();

        void doCancel();

        int Execute(float dt);
		virtual const char* GetName() { return "CampaignMenuState"; }

    private:
        VBox* m_vbox;
        
        BoringButton* m_butCancel;
		ImagePtr m_surf;

};

#endif // DUNE_CAMPAIGNMENU_H
