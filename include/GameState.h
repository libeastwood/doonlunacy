#ifndef DUNE_GAMESTATE_H
#define DUNE_GAMESTATE_H

#include "MapGenerator.h"
#include "MapClass.h"
#include "PlayerClass.h"
#include "structures/StructureClass.h"
#include "units/UnitClass.h"

typedef std::vector<PlayerClass*> Players;
typedef std::vector<StructureClass*> Structures;
typedef std::vector<UnitClass*> Units;

class GameState : public Singleton<GameState>
{
  friend class Singleton<GameState>;
  friend class MapClass;
  friend class MapGenerator;

  public:
    GameState();
    ~GameState();
    
    MapClass* GetMap() { return m_map; }
    Players* GetPlayers() { return m_players; }
    Structures* GetStructures() { return m_structures; }
    Units* GetUnits()  { return m_units; }
    
    //TODO:This needs to be done in a smarter way
    Uint32 getObjectID() { return ++m_objectID; fprintf(stderr, "ObjectID %d", m_objectID); }

    PlayerClass* LocalPlayer() { return m_localPlayer; }; 
  protected:

    MapClass* m_map;
    PlayerClass * m_localPlayer;
    Players* m_players;
    Structures* m_structures;
    Units* m_units;
    Uint32 m_objectID;

  
};

#endif // DUNE_GAMESTATE_H

