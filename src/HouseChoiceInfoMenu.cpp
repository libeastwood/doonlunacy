#include "HouseChoiceInfoMenu.h"

#include "Application.h"
#include "Settings.h"
#include "DataCache.h"
#include "boost/bind.hpp"
#include "Animation.h"
#include "gui2/Label.h"
#include "gui2/Button.h"
#include "pakfile/Stringfile.h"
#include <string>

HouseChoiceInfoMenuState::HouseChoiceInfoMenuState(HOUSETYPE newHouse) : MentatMenuState(HOUSE_SARDAUKAR)
{
	Animation* ret;
	if (newHouse == HOUSE_ATREIDES)
		ret = DataCache::Instance()->getAnimation(Anim_AtreidesPlanet);
	else if (newHouse == HOUSE_ORDOS)
		ret = DataCache::Instance()->getAnimation(Anim_OrdosPlanet);
	else if (newHouse == HOUSE_HARKONNEN)
		ret = DataCache::Instance()->getAnimation(Anim_HarkonnenPlanet);

	
	m_planetAnimation = new AnimationLabel(ret);
	m_planetAnimation->setPosition(UPoint(256,126));
	m_container->addChild(m_planetAnimation);

    m_butYes = new GraphicButton(DataCache::Instance()->getGuiPic(UI_MentatYes)->getResized(2), DataCache::Instance()->getGuiPic(UI_MentatYes_Pressed)->getResized(2));
	m_butYes->setPosition(UPoint(336,366));
	m_container->addChild(m_butYes);

    m_butNo = new GraphicButton(DataCache::Instance()->getGuiPic(UI_MentatNo)->getResized(2), DataCache::Instance()->getGuiPic(UI_MentatNo_Pressed)->getResized(2));
	m_butNo->setPosition(UPoint(480,366));
    m_butNo->onClick.connect(
        boost::bind(&HouseChoiceInfoMenuState::doNo, this) );    
    m_container->addChild(m_butNo);

	std::string desc = DataCache::Instance()->getBriefingText(0, MISSION_DESCRIPTION, newHouse);
	int linebreak = desc.find("\n",0) + 1;
	m_textLabel = new Label(desc.substr(linebreak,desc.length()-linebreak), 158, -1, 64); //DataCache::Instance()->getBriefingText(0, MISSION_DESCRIPTION, newHouse), 0);
	m_textLabel->setPosition(UPoint(0,35));
	m_container->addChild(m_textLabel);
	drawSpecificStuff();

}

HouseChoiceInfoMenuState::~HouseChoiceInfoMenuState()
{
}

void HouseChoiceInfoMenuState::doNo()
{
    assert(mp_parent != NULL);
    PopState();
}
