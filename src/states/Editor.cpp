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

#if 1
    m_mapWidget = new MapWidget();
    m_mapWidget->setPosition(UPoint(0,56));
    m_mapWidget->setSize(UPoint(set->GetWidth() - 144, set->GetHeight() - m_mapWidget->getPosition().y));
    GameMan::Instance()->LoadScenario("SCENARIO:SCENA001.INI");
    //FIXME: Should really use m_backgroundFrame..
    m_container->addChild(m_mapWidget);
#endif 
}
