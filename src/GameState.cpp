#include "GameState.h"

GameState::GameState()
{
    m_players = new Players;
    m_structures = new Structures;
    m_units = new Units;
	m_objectTree = new ObjectTree();
}

GameState::~GameState()
{

}

void GameState::Update()
{
    Units::iterator iter;
    for (iter = m_units->begin(); iter != m_units->end(); iter++)
    {
        (*iter)->update();
    }
}
