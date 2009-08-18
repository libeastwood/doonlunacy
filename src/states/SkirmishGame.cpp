#include "states/SkirmishGame.h"

#include "Settings.h"
#include "MapGenerator.h"
#include "MapClass.h"

#include "gui2/Container.h"
#include "gui2/Frame.h"

SkirmishGameState::SkirmishGameState(HOUSETYPE house, int level) : GameMenuState(house), m_level(level)
{
    m_mapWidget = new MapWidget();
}

void SkirmishGameState::draw()
{
    m_backgroundFrame->addChild(m_mapWidget);

    GameMenuState::draw();

}

void SkirmishGameState::resize()
{
    GameMenuState::resize();
    m_mapWidget->setPosition(UPoint(0, m_topFrame->getSize().y));
    UPoint resolution = set->GetResolution();
    UPoint mapSize(resolution.x - m_sideBarFrame->getSize().x, resolution.y - m_mapWidget->getPosition().y);
    mapSize.x -= mapSize.x % BLOCKSIZE;
    mapSize.y -= mapSize.y % BLOCKSIZE;    

    mapSize += BLOCKSIZE;

    m_mapWidget->setSize(mapSize);

    char mission[] = "SCENARIO:SCEN%c%.3d.INI";
    char house;
    if(m_house == HOUSE_HARKONNEN)
	house = 'H';
    else if(m_house == HOUSE_ATREIDES)
	house = 'A';
    else if(m_house == HOUSE_ORDOS)
	house = 'O';
    else if(m_house == HOUSE_FREMEN)
	house = 'F';
    else if(m_house == HOUSE_MERCENARY)
	house = 'M';
    else
	house = 0;
    sprintf(mission, mission, house, m_level);

    GameMan::Instance()->LoadScenario(mission);
    //TODO: adjust to make it relative to current resolution rather than 320x200
    m_mapWidget->setMapPosition(GameMan::Instance()->getTacticalPos());
}
