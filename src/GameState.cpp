#include "GameState.h"

GameState::GameState()
{
    m_players = new Players;
    m_structures = new Structures;
    m_units = new Units;
    m_objectID = 0;

}

GameState::~GameState()
{

}

