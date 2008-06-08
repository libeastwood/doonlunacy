#include "Editor.h"
#include "MapGenerator.h"
#include "MapClass.h"

EditorState::EditorState()
{
    m_mapWidget = new MapWidget();
    m_mapWidget->setPosition(UPoint(0,0));
    m_mapWidget->setSize(UPoint(640,480));

    MapGenerator::Instance()->loadOldMap("SCENARIO:SCENA021.INI");
//   MapGenerator::Instance()->makeRandomMap(UPoint(100,100));

    MapClass * map = MapGenerator::Instance()->getMap();
    
    m_mapWidget->setMap(map);

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
