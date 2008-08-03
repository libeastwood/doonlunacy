#include <boost/bind.hpp>

#include "Application.h"
#include "DataCache.h"
#include "HouseChoiceMenu.h"
#include "HouseChoiceInfoMenu.h"
#include "Settings.h"

#include "gui2/Button.h"
HouseChoiceMenuState::HouseChoiceMenuState() : MenuBaseState()
{
    m_menuBackground = DataCache::Instance()->getGuiPic(UI_HouseChoiceBackground)->getResized();

    m_butAtreides = new TranspButton(sizeRelativeUPoint(84,92));
    m_butAtreides->onClick.connect(
            boost::bind(&HouseChoiceMenuState::doAtreides, this) );

    m_butAtreides->setPosition(sizeRelativeUPoint(20,53));
    
    m_container->addChild(m_butAtreides);

    m_butOrdos = new TranspButton(sizeRelativeUPoint(84, 92));
    m_butOrdos->onClick.connect(
            boost::bind(&HouseChoiceMenuState::doOrdos, this) );

    m_butOrdos->setPosition(sizeRelativeUPoint(116,53));
    m_container->addChild(m_butOrdos);

    m_butHarkonnen = new TranspButton(sizeRelativeUPoint(84, 92));
    m_butHarkonnen->onClick.connect(
            boost::bind(&HouseChoiceMenuState::doHarkonnen, this) );

    m_butHarkonnen->setPosition(sizeRelativeUPoint(215, 53));
    m_container->addChild(m_butHarkonnen);
    
    m_butBack = new BoringButton("Back to menu", false);
    m_butBack->setPosition(SPoint(30, Settings::Instance()->GetHeight() - m_butBack->h - 50));
    m_butBack->setSize(UPoint(180, 35));
    m_butBack->onClick.connect(
             boost::bind(&HouseChoiceMenuState::doBack, this) );
    m_container->addChild(m_butBack);        

}

HouseChoiceMenuState::~HouseChoiceMenuState() {
}

int HouseChoiceMenuState::Execute(float dt)
{
    m_menuBackground->blitToScreen();

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
