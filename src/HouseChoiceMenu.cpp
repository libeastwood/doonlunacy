#include "HouseChoiceMenu.h"
#include "HouseChoiceInfoMenu.h"

#include "DataCache.h"
#include "Application.h"
#include "DataCache.h"
#include "boost/bind.hpp"
#include <iostream>
#include "gui2/Button.h"
#include "gui2/VBox.h"
HouseChoiceMenuState::HouseChoiceMenuState() : MenuBaseState()
{
    m_menuBackground.reset(DataCache::Instance()->getGuiPic(UI_HouseChoiceBackground).get());
    m_menuBackground = m_menuBackground->getResized(2);

    m_vbox = new VBox();

	m_butAtreides = new TranspButton(163,182);
    m_butAtreides->onClick.connect(
            boost::bind(&HouseChoiceMenuState::doAtreides, this) );

    m_vbox->addChild(m_butAtreides);

    m_vbox->fit(2);
	m_vbox->setPosition(UPoint(40,135));
    m_vbox->reshape();
    
    m_container->addChild(m_vbox);

    m_vbox = new VBox();

	m_butOrdos = new TranspButton(163,182);
    m_butOrdos->onClick.connect(
            boost::bind(&HouseChoiceMenuState::doOrdos, this) );

    m_vbox->addChild(m_butOrdos);

    m_vbox->fit(2);
	m_vbox->setPosition(UPoint(235,135));
    m_vbox->reshape();
    
    m_container->addChild(m_vbox);

    m_vbox = new VBox();

	m_butHarkonnen = new TranspButton(163,182);
    m_butHarkonnen->onClick.connect(
            boost::bind(&HouseChoiceMenuState::doHarkonnen, this) );

    m_vbox->addChild(m_butHarkonnen);

    m_vbox->fit(2);
	m_vbox->setPosition(UPoint(430,135));
    m_vbox->reshape();
    
    m_container->addChild(m_vbox);

}

HouseChoiceMenuState::~HouseChoiceMenuState() {
}

int HouseChoiceMenuState::Execute(float dt)
{
    m_menuBackground.get()->blitToScreen(SPoint(Settings::Instance()->GetWidth() / 2 - m_menuBackground->getSurface()->w/2, 
                        Settings::Instance()->GetHeight() / 16));

    return 0;
}

void HouseChoiceMenuState::doAtreides() {
	std::cout << "ATREIDES!!" << std::endl;
    mp_parent->PushState( new HouseChoiceInfoMenuState(HOUSE_ATREIDES) );
/*	HouseChoiceInfoMenu* myHouseChoiceInfoMenu = new HouseChoiceInfoMenu(HOUSE_ATREIDES);
	if(myHouseChoiceInfoMenu == NULL) {
		perror("HouseChoiceMenu::OnAtreides()");
		exit(EXIT_FAILURE);
	}
	if(myHouseChoiceInfoMenu->showMenu() == -1) {
		quit(-1);
	} else {
		quit(HOUSE_ATREIDES);
	}
	delete myHouseChoiceInfoMenu;*/
}

void HouseChoiceMenuState::doOrdos() {
	std::cout << "ORDOS!!" << std::endl;

    mp_parent->PushState( new HouseChoiceInfoMenuState(HOUSE_ORDOS) );
/*	HouseChoiceInfoMenu* myHouseChoiceInfoMenu = new HouseChoiceInfoMenu(HOUSE_ORDOS);
	if(myHouseChoiceInfoMenu == NULL) {
		perror("HouseChoiceMenu::OnOrdos()");
		exit(EXIT_FAILURE);
	}
	if(myHouseChoiceInfoMenu->showMenu() == -1) {
		quit(-1);
	} else {
		quit(HOUSE_ORDOS);
	}
	delete myHouseChoiceInfoMenu;*/
}

void HouseChoiceMenuState::doHarkonnen() {
	std::cout << "HARKONNEN!!" << std::endl;

    mp_parent->PushState( new HouseChoiceInfoMenuState(HOUSE_HARKONNEN) );
/*	HouseChoiceInfoMenu* myHouseChoiceInfoMenu = new HouseChoiceInfoMenu(HOUSE_HARKONNEN);
	if(myHouseChoiceInfoMenu == NULL) {
		perror("HouseChoiceMenu::OnHarkonnen()");
		exit(EXIT_FAILURE);
	}
	if(myHouseChoiceInfoMenu->showMenu() == -1) {
		quit(-1);
	} else {
		quit(HOUSE_HARKONNEN);
	}
	delete myHouseChoiceInfoMenu;*/
}

