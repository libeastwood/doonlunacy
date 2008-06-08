#include "Settings.h"
#include "Editor.h"
#include "MapGenerator.h"
#include "MapClass.h"

EditorState::EditorState()
{
    Settings * set = Settings::Instance();
    m_mapWidget = new MapWidget();
    m_mapWidget->setPosition(UPoint(0,0));
    m_mapWidget->setSize(UPoint(set->GetWidth(),set->GetHeight()/2 - 20));

    m_mapWidget2 = new MapWidget();
    m_mapWidget2->setPosition(UPoint(0,set->GetHeight()/2));
    m_mapWidget2->setSize(UPoint(set->GetWidth(),set->GetHeight()/2 - 20));
    
    MapGenerator::Instance()->loadOldMap("SCENARIO:SCENA010.INI");
    MapClass * map = MapGenerator::Instance()->getMap();
    m_mapWidget->setMap(map);
    m_mapWidget->setStructureList(MapGenerator::Instance()->getStructureList());

    MapGenerator::Instance()->loadOldMap("SCENARIO:SCENH010.INI");
    map = MapGenerator::Instance()->getMap();
    m_mapWidget2->setMap(map);
    m_mapWidget2->setStructureList(MapGenerator::Instance()->getStructureList());


    m_container->addChild(m_mapWidget2);
    m_container->addChild(m_mapWidget);
}

EditorState::~EditorState()
{

}

int EditorState::Execute(float dt)
{
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
