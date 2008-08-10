#include "states/SkirmishMenu.h"

#include "DataCache.h"
#include "DrawImage.h"
#include "GCObject.h"

#include "gui2/Button.h"
#include "gui2/Frame.h"
#include "gui2/VBox.h"

#include <boost/bind.hpp>


SkirmishMenuState::SkirmishMenuState() : MainMenuBaseState()
{
    m_butStart = new BoringButton("Start");
    m_butStart->setSize(SPoint(bw, bh));
    m_butStart->onClick.connect(
            boost::bind(&SkirmishMenuState::doStart, this) );
    m_vbox->addChild(m_butStart);

    m_butCancel = new BoringButton("Cancel");
    m_butCancel->setSize(SPoint(bw, bh));
    m_butCancel->onClick.connect(
            boost::bind(&SkirmishMenuState::doCancel, this) );
    m_vbox->addChild(m_butCancel);

}

void SkirmishMenuState::drawSpecifics()
{
	DataCache::Instance()->getGCObject("BigPlanet")->getImage();

    ImagePtr houseChoice = DataCache::Instance()->getGCObject("UI_HouseChoiceBackground")->getImage();
    ((DrawImage*)(houseChoice.get()))->drawBorders1();
    m_middleFrame->changeBackground(houseChoice);
}

void SkirmishMenuState::doStart()
{
}

void SkirmishMenuState::doCancel()
{
    assert(mp_parent != NULL);
    PopState();
}
