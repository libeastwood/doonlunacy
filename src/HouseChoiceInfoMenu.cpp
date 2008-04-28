#include "HouseChoiceInfoMenu.h"

#include "Application.h"
#include "Settings.h"
#include "DataCache.h"
#include "pakfile/Cpsfile.h"
#include "boost/bind.hpp"

HouseChoiceInfoMenuState::HouseChoiceInfoMenuState(HOUSETYPE newHouse) : MentatMenuState(HOUSE_SARDAUKAR)
{

    m_butYes = new GraphicButton(DataCache::Instance()->getGuiPic(UI_MentatYes)->getResized(2), DataCache::Instance()->getGuiPic(UI_MentatYes_Pressed)->getResized(2));
    m_vbox->addChild(m_butYes);

    m_vbox->fit(2);
	m_vbox->setPosition(UPoint(336,366));
    m_vbox->reshape();
    
    m_container->addChild(m_vbox);

    m_vbox = new VBox();

    m_butNo = new GraphicButton(DataCache::Instance()->getGuiPic(UI_MentatNo)->getResized(2), DataCache::Instance()->getGuiPic(UI_MentatNo_Pressed)->getResized(2));
    m_vbox->addChild(m_butNo);

    m_vbox->fit(2);
	m_vbox->setPosition(UPoint(480,366));
    m_vbox->reshape();
    
    m_container->addChild(m_vbox);
}

HouseChoiceInfoMenuState::~HouseChoiceInfoMenuState()
{
}
