#include "states/HouseChoiceMenu.h"

#include "DataCache.h"
#include "GameData.h"
#include "Gfx.h"
#include "Settings.h"

#include "gui2/Button.h"
#include "gui2/Frame.h"

#include "states/HouseChoiceInfoMenu.h"

#include <boost/bind.hpp>

HouseChoiceMenuState::HouseChoiceMenuState() : MenuBaseState()
{
    ImagePtr background = DataCache::Instance()->getGameData("UI_HouseChoiceBackground")->getImage()->getResized();
    
	m_backgroundFrame->changeBackground(background);

    m_butAtreides = new TransparentButton(UPoint(84,92).getScaled());
    m_butAtreides->onClick.connect(
            boost::bind(&HouseChoiceMenuState::doAtreides, this) );

    m_butAtreides->setPosition(UPoint(20,53).getScaled());
    m_backgroundFrame->addChild(m_butAtreides);

    m_butOrdos = new TransparentButton(UPoint(84, 92).getScaled());
    m_butOrdos->onClick.connect(
            boost::bind(&HouseChoiceMenuState::doOrdos, this) );

    m_butOrdos->setPosition(UPoint(116,53).getScaled());
    m_backgroundFrame->addChild(m_butOrdos);

    m_butHarkonnen = new TransparentButton(UPoint(84, 92).getScaled());
    m_butHarkonnen->onClick.connect(
            boost::bind(&HouseChoiceMenuState::doHarkonnen, this) );

    m_butHarkonnen->setPosition(UPoint(215, 53).getScaled());
    m_backgroundFrame->addChild(m_butHarkonnen);
    
    m_butBack = new BoringButton("Back to menu", false);
    m_butBack->setPosition(SPoint(30, Settings::Instance()->GetHeight() - m_butBack->h - 50));
    m_butBack->setSize(UPoint(180, 35));
    m_butBack->onClick.connect(
             boost::bind(&HouseChoiceMenuState::doBack, this) );
    m_backgroundFrame->addChild(m_butBack);        
}

HouseChoiceMenuState::~HouseChoiceMenuState() {
}

int HouseChoiceMenuState::Execute(float dt)
{
    return 0;
}

void HouseChoiceMenuState::doAtreides() {
    mp_parent->PushState( new HouseChoiceInfoMenuState(HOUSE_ATREIDES) );
}

void HouseChoiceMenuState::doOrdos() {
    mp_parent->PushState( new HouseChoiceInfoMenuState(HOUSE_ORDOS) );
}

void HouseChoiceMenuState::doHarkonnen() {
    mp_parent->PushState( new HouseChoiceInfoMenuState(HOUSE_HARKONNEN) );
}

void HouseChoiceMenuState::doBack() {
	assert(mp_parent != NULL);
	PopState();
}
