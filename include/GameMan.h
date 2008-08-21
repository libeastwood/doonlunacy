#ifndef DUNE_GAMEMANAGER_H
#define DUNE_GAMEMANAGER_H

#include "BulletClass.h"
#include "MapGenerator.h"
#include "MapClass.h"
#include "ObjectTree.h"
#include "PlayerClass.h"
#include "singleton.h"

#include "structures/StructureClass.h"
#include "units/UnitClass.h"

typedef std::vector<PlayerClass*> Players;
typedef std::vector<StructureClass*> Structures;
typedef std::list<UnitClass*> Units;
typedef std::vector<BulletClass*> Bullets;

//! @note GameMan pronounced G-Man http://en.wikipedia.org/wiki/G-Man_%28Half-Life%29

class GameMan : public Singleton<GameMan>
{
  friend class Singleton<GameMan>;
  friend class MapClass;
  friend class MapGenerator;

  public:
    GameMan();
    ~GameMan();
    
    void Init();
    void Clear();
    
    MapClass* GetMap() { return m_map; }
    Players* GetPlayers() { return m_players; }
    PlayerClass* GetPlayer(int i) { return m_players->at(i); }
    Structures* GetStructures() { return m_structures; }
    Units* GetUnits()  { return m_units; }
    Bullets* GetBullets() { return m_bulletList; }    
    PlayerClass* LocalPlayer() { return m_localPlayer; }
    ObjectTree* GetObjectTree() { return m_objectTree; }
    void Update(float dt);
	//FIXME: This shouldn't be here
	bool placingMode;
  protected:

    Bullets* m_bulletList;
    MapClass* m_map;
    ObjectTree* m_objectTree;
    PlayerClass * m_localPlayer;
    Players* m_players;
    Structures* m_structures;

    Units* m_units;
};

#endif // DUNE_GAMEMANAGER_H

