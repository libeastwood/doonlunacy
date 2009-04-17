#include "states/HouseChoiceInfoMenu.h"

#include "DataCache.h"
#include "GameData.h"

#include "gui2/Frame.h"
#include "gui2/Label.h"
#include "gui2/Button.h"

#include <eastwood/StringFile.h>

#include <string>
#include <boost/bind.hpp>

HouseChoiceInfoMenuState::HouseChoiceInfoMenuState(HOUSETYPE newHouse) : MentatMenuState(HOUSE_SARDAUKAR)
{

	delete m_song;
	m_song = NULL;
	if (newHouse == HOUSE_ATREIDES)
		m_planetAnimation = DataCache::Instance()->getAnimationLabel("Anim_AtreidesPlanet");
	else if (newHouse == HOUSE_ORDOS)
		m_planetAnimation = DataCache::Instance()->getAnimationLabel("Anim_OrdosPlanet");
	else if (newHouse == HOUSE_HARKONNEN)
		m_planetAnimation = DataCache::Instance()->getAnimationLabel("Anim_HarkonnenPlanet");

	
	m_planetAnimation->setPosition(UPoint(128, 48).getScaled());
	m_backgroundFrame->addChild(m_planetAnimation);

	m_butYes = new GraphicButton(DataCache::Instance()->getGameData("UI_MentatYes")->getImage()->getResized(), DataCache::Instance()->getGameData("UI_MentatYes_Pressed")->getImage()->getResized());
	m_butYes->setPosition(UPoint(168, 168).getScaled());
	m_backgroundFrame->addChild(m_butYes);

	m_butNo = new GraphicButton(DataCache::Instance()->getGameData("UI_MentatNo")->getImage()->getResized(), DataCache::Instance()->getGameData("UI_MentatNo_Pressed")->getImage()->getResized());
	m_butNo->setPosition(UPoint(240, 168).getScaled());
	m_butNo->onClick.connect(
        boost::bind(&HouseChoiceInfoMenuState::doNo, this) );    
        m_backgroundFrame->addChild(m_butNo);

	std::string desc = DataCache::Instance()->getBriefingText(0, MISSION_DESCRIPTION, newHouse);
	int linebreak = desc.find("\n",0) + 1;
	m_textLabel = new TransparentLabel(desc.substr(linebreak,desc.length()-linebreak), 158, 64);
	m_textLabel->setPosition(UPoint(0,35));
	m_backgroundFrame->addChild(m_textLabel);
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
