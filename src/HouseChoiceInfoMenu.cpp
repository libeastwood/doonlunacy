#include "HouseChoiceInfoMenu.h"

#include "Application.h"
#include "Settings.h"
#include "DataCache.h"
#include "boost/bind.hpp"
#include "Animation.h"
#include "gui2/Label.h"
#include "gui2/Button.h"
#include <StringFile.h>
#include <string>

HouseChoiceInfoMenuState::HouseChoiceInfoMenuState(HOUSETYPE newHouse) : MentatMenuState(HOUSE_SARDAUKAR)
{

	free(m_song);
	m_song = NULL;
	Animation* ret;
	if (newHouse == HOUSE_ATREIDES)
		ret = DataCache::Instance()->getAnimation("Anim_AtreidesPlanet");
	else if (newHouse == HOUSE_ORDOS)
		ret = DataCache::Instance()->getAnimation("Anim_OrdosPlanet");
	else if (newHouse == HOUSE_HARKONNEN)
		ret = DataCache::Instance()->getAnimation("Anim_HarkonnenPlanet");

	
	m_planetAnimation = new AnimationLabel(ret);
	m_planetAnimation->setPosition(UPoint(128, 48).getScaled());
	m_container->addChild(m_planetAnimation);

	m_butYes = new GraphicButton(DataCache::Instance()->getGuiPic(UI_MentatYes)->getResized(), DataCache::Instance()->getGuiPic(UI_MentatYes_Pressed)->getResized());
	m_butYes->setPosition(UPoint(168, 168).getScaled());
	m_container->addChild(m_butYes);

	m_butNo = new GraphicButton(DataCache::Instance()->getGuiPic(UI_MentatNo)->getResized(), DataCache::Instance()->getGuiPic(UI_MentatNo_Pressed)->getResized());
	m_butNo->setPosition(UPoint(240, 168).getScaled());
	m_butNo->onClick.connect(
        boost::bind(&HouseChoiceInfoMenuState::doNo, this) );    
	    m_container->addChild(m_butNo);

	std::string desc = DataCache::Instance()->getBriefingText(0, MISSION_DESCRIPTION, newHouse);
	int linebreak = desc.find("\n",0) + 1;
	m_textLabel = new Label(desc.substr(linebreak,desc.length()-linebreak), 158, -1, 64);
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
