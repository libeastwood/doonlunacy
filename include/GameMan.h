#ifndef DUNE_GAMEMANAGER_H
#define DUNE_GAMEMANAGER_H

#include "MapGenerator.h"
#include "MapClass.h"
#include "ObjectTree.h"
#include "PlayerClass.h"
#include "singleton.h"

#include "objects/StructureClass.h"
#include "objects/UnitClass.h"
#include "objects/WeaponClass.h"

typedef std::list <Uint32> List;

typedef std::vector<PlayerClass*> Players;
typedef std::vector<ObjectClass*> Objects;

//! @note GameMan pronounced G-Man http://en.wikipedia.org/wiki/G-Man_%28Half-Life%29

extern int lookDist[11];

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
    bool LoadScenario(std::string scenarioName);
    
    void AddPlayer(PLAYERHOUSE House, bool ai, int team);
    ObjectClass* createObject(std::string itemName, PlayerClass* Owner, Uint32 ObjectID = NONE);

    /*!
     *  As the name suggests it saves current map to a BMP file.
     *  Currently only terrain without units, buildings, etc.
     *  @param filename of file to which mapshot should be saved. By default: mapshot.bmp
     */

    void TakeMapScreenshot(std::string filename = "mapshot.bmp");
    
    bool SplitString(std::string ParseString, unsigned int NumStringPointers, ...);
    
    MapClass* GetMap() { return m_map; }
    Players* GetPlayers() { return m_players; }
    PlayerClass* GetPlayer(int i) { return m_players->at(i); }
    Objects* GetObjects() { return m_objects; }
    PlayerClass* LocalPlayer() { return m_localPlayer; }
    ObjectTree* GetObjectTree() { return m_objectTree; }
    void Update(float dt);
	//FIXME: This shouldn't be here
	bool placingMode;
	
	void Select(List* objectList);
	void Unselect(List* objectList);
  protected:

    MapClass* m_map;
    ObjectTree* m_objectTree;
    PlayerClass * m_localPlayer;
    Players* m_players;
    Objects* m_objects;
};

#endif // DUNE_GAMEMANAGER_H

