#include "CampaignMenu.h"

#include "Application.h"
#include "Settings.h"
#include "DataCache.h"
#include "pakfile/Cpsfile.h"
#include "boost/bind.hpp"


CampaignMenuState::CampaignMenuState()
{
	int len;
    unsigned char * data = ResMan::Instance()->readFile("ENGLISH:HERALD.ENG", &len);

    CpsfilePtr m_cps (new Cpsfile(data, len));

    m_surf.reset(m_cps->getPicture());
    m_surf = m_surf->getResized(2);

}

CampaignMenuState::~CampaignMenuState()
{
}

void CampaignMenuState::doCancel()
{
    assert(mp_parent != NULL);
    PopState();
}

int CampaignMenuState::Execute(float dt)
{
    m_surf.get()->blitToScreen(SPoint(Settings::Instance()->GetWidth() / 2 - m_surf->getSurface()->w/2, 
                        Settings::Instance()->GetHeight() / 8));

    return 0;
}

