#include "Editor.h"
#include "MapGenerator.h"
#include "gui2/Container.h"

EditorState::EditorState()
{
    MapGenerator::Instance()->loadOldMap("SCENARIO:SCENH022.INI");
    m_mapWidget->setGameState(MapGenerator::Instance()->getGameState());
    m_container->addChild(m_mapWidget);
}


EditorState::~EditorState()
{

}
