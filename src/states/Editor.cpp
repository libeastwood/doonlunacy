#include "states/Editor.h"

#include "Settings.h"
#include "MapGenerator.h"
#include "MapClass.h"

#include "gui2/Container.h"
#include "gui2/Frame.h"

EditorState::EditorState(HOUSETYPE house) : GameMenuState(house)
{
#if 0
    m_mapWidget = new MapWidget();
    m_mapWidget->setPosition(UPoint(0,0));
    m_mapWidget->setSize(UPoint(set->GetWidth(),set->GetHeight()/2 - 20));

    m_mapWidget2 = new MapWidget();
    m_mapWidget2->setPosition(UPoint(0,set->GetHeight()/2));
    m_mapWidget2->setSize(UPoint(set->GetWidth(),set->GetHeight()/2 - 20));
    
    MapGenerator::Instance()->loadOldMap("SCENARIO:SCENH019.INI");
    m_mapWidget->setGameState(MapGenerator::Instance()->getGameState());


    MapGenerator::Instance()->loadOldMap("SCENARIO:SCENA019.INI");
    m_mapWidget2->setGameState(MapGenerator::Instance()->getGameState());

    m_container->addChild(m_mapWidget);
//    m_container->addChild(m_mapWidget2);
#endif

    m_mapWidget = new MapWidget();
}

void EditorState::draw()
{
    m_backgroundFrame->addChild(m_mapWidget);

    GameMenuState::draw();

}

void EditorState::resize()
{
    GameMenuState::resize();
    m_mapWidget->setPosition(UPoint(0, m_topFrame->getSize().y));
    UPoint resolution = set->GetResolution();
    UPoint mapSize(resolution.x - m_sideBarFrame->getSize().x, resolution.y - m_mapWidget->getPosition().y);
    mapSize.x -= mapSize.x % BLOCKSIZE;
    mapSize.y -= mapSize.y % BLOCKSIZE;    

    mapSize += BLOCKSIZE;

    m_mapWidget->setSize(mapSize);
    GameMan::Instance()->LoadScenario("SCENARIO:SCENA001.INI");
}
