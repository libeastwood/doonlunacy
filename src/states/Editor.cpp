#include "Settings.h"
#include "MapGenerator.h"
#include "MapClass.h"

#include "states/Editor.h"

EditorState::EditorState()
{
    Settings * set = Settings::Instance();

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
    m_mapWidget->setPosition(UPoint(0,0));
    m_mapWidget->setSize(UPoint(set->GetWidth(),set->GetHeight()));
    MapGenerator::Instance()->loadOldMap("SCENARIO:SCENA005.INI");
    m_mapWidget->getGameState();
    m_container->addChild(m_mapWidget);
#endif 
}


EditorState::~EditorState()
{

}

int EditorState::Execute(float dt)
{
   GameState::Instance()->Update();
   SDL_Event event;
   SDL_PollEvent(&event);
   if (event.type == SDL_KEYDOWN) {
	   switch (event.key.keysym.sym) {
		case SDLK_ESCAPE : PopState(); break;
		default : break;
	   }
   }
    return 0;
}
