#include "SkirmishMenu.h"

#include "Application.h"
#include "houses.h"
#include "DataCache.h"
#include "Settings.h"

#include "gui2/Frame.h"

#include <boost/bind.hpp>


SkirmishMenuState::SkirmishMenuState() : MainMenuBaseState()
{
    m_butCancel = new BoringButton("Cancel");
    m_butCancel->setSize(SPoint(bw, bh));
    m_butCancel->onClick.connect(
            boost::bind(&SkirmishMenuState::doCancel, this) );
   
    m_vbox->addChild(m_butCancel);

}

SkirmishMenuState::~SkirmishMenuState()
{
}


void SkirmishMenuState::drawMainImage(){
    Uint16 x = (Settings::Instance()->GetWidth() / 2) - 
                (m_vbox->w / 2);
    Frame1 housechoice(DataCache::Instance()->getGuiPic(UI_HouseChoiceBackground).get(), UPoint(320, 200));
//    housechoice.getPicture()->recolor(0, 236);

    m_menuBackground->blitFrom(housechoice.getPicture().get(), UPoint(x - 70, Settings::Instance()->GetHeight()/2 - 172));
}

void SkirmishMenuState::doCancel()
{
    assert(mp_parent != NULL);
    PopState();
}

