#ifndef DUNE_GAMESTATE_H
#define DUNE_GAMESTATE_H

#include "MapClass.h"
#include "PlayerClass.h"
#include "structures/StructureClass.h"
#include "units/UnitClass.h"

class GameState
{
  public:
    GameState();
    ~GameState();

    MapClass* m_map;
    Players* m_players;
    Structures* m_structures;
    Units* m_units;
    
    Uint32 getObjectID() { return ++m_objectID; fprintf(stderr, "ObjectID %d", m_objectID); }
  
  private:
    Uint32 m_objectID;

  
};

#endif // DUNE_GAMESTATE_H

