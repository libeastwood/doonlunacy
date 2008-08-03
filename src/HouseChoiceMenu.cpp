#include "boost/bind.hpp"

#include "Application.h"
#include "DataCache.h"
#include "HouseChoiceMenu.h"
#include "HouseChoiceInfoMenu.h"
#include "Settings.h"

#include "gui2/Button.h"
#include "gui2/VBox.h"
HouseChoiceMenuState::HouseChoiceMenuState() : MenuBaseState()
{
    m_menuBackground = DataCache::Instance()->getGuiPic(UI_HouseChoiceBackground)->getResized();
    m_vbox = new VBox();

    m_butAtreides = new TranspButton(sizeRelativePoint(UPoint(84,92)));
    m_butAtreides->onClick.connect(
            boost::bind(&HouseChoiceMenuState::doAtreides, this) );

    m_vbox->addChild(m_butAtreides);

    m_vbox->fit(2);
    m_vbox->setPosition(sizeRelativePoint(UPoint(20,53)));
    m_vbox->reshape();
    
    m_container->addChild(m_vbox);

    m_vbox = new VBox();

    m_butOrdos = new TranspButton(sizeRelativePoint(UPoint(84, 92)));
    m_butOrdos->onClick.connect(
            boost::bind(&HouseChoiceMenuState::doOrdos, this) );

    m_vbox->addChild(m_butOrdos);

    m_vbox->fit(2);
	m_vbox->setPosition(sizeRelativePoint(UPoint(116,53)));
    m_vbox->reshape();
    
    m_container->addChild(m_vbox);

    m_vbox = new VBox();

    m_butHarkonnen = new TranspButton(sizeRelativePoint(UPoint(84, 92)));
    m_butHarkonnen->onClick.connect(
            boost::bind(&HouseChoiceMenuState::doHarkonnen, this) );

    m_vbox->addChild(m_butHarkonnen);

    m_vbox->fit(2);
    m_vbox->setPosition(sizeRelativePoint(UPoint(215, 53)));
    m_vbox->reshape();
    
    
    m_butBack = new BoringButton("Back to menu", false);
    m_butBack->setPosition(SPoint(30, Settings::Instance()->GetHeight() - m_butBack->h - 50));
    m_butBack->setSize(UPoint(180, 35));
    m_butBack->onClick.connect(
             boost::bind(&HouseChoiceMenuState::doBack, this) );
    m_container->addChild(m_butBack);        
    
    m_container->addChild(m_vbox);


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
