#ifndef DUNE_GAMESTATE_H
#define DUNE_GAMESTATE_H

#include "MapGenerator.h"
#include "MapClass.h"
#include "ObjectTree.h"
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
    PlayerClass* GetPlayer(int i) { return m_players->at(i); }
    Structures* GetStructures() { return m_structures; }
    Units* GetUnits()  { return m_units; }
    
    PlayerClass* LocalPlayer() { return m_localPlayer; }; 
    ObjectTree* GetObjectTree() { return m_objectTree; };
    void Update();
	//FIXME: This shouldn't be here
	bool placingMode;
  protected:

    MapClass* m_map;
    ObjectTree* m_objectTree;
    PlayerClass * m_localPlayer;
    Players* m_players;
    Structures* m_structures;
    Units* m_units;
};

#endif // DUNE_GAMESTATE_H

