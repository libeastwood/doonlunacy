#ifndef DUNE_GAMEMANAGER_H
#define DUNE_GAMEMANAGER_H

#include <map>

#include "MapGenerator.h"
#include "MapClass.h"
#include "PlayerClass.h"
#include "singleton.h"
#include "houses.h"

#include "objects/StructureClass.h"
#include "objects/UnitClass.h"
#include "objects/WeaponClass.h"

//! @note GameMan pronounced G-Man http://en.wikipedia.org/wiki/G-Man_%28Half-Life%29

extern uint8_t lookDist[11];

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
    
    void AddPlayer(HOUSETYPE House, bool ai, int team);
    ObjectPtr createObject(std::string itemName, const PlayerClass& Owner);

    /*!
     *  As the name suggests it saves current map to a BMP file.
     *  Currently only terrain without units, buildings, etc.
     *  @param filename of file to which mapshot should be saved. By default: mapshot.bmp
     */

    void TakeMapScreenshot(std::string filename = "mapshot.bmp");
    
    MapClass * GetMap() const noexcept { return m_map; }
    const std::vector<PlayerClass>& GetPlayers() { return m_players; }
    inline bool playerExists(uint8_t i) { return i < m_players.size(); }
    const PlayerClass& GetPlayer(int i) { return m_players[i]; }
    inline ObjectTypeMap::const_iterator getObjectsBegin() { return m_objects.begin(); }
    inline ObjectTypeMap::const_iterator getObjectsEnd() { return m_objects.end(); }
    ObjectPtr getObject(uint32_t objectID);
    uint32_t addObject(ObjectPtr object);
    void removeObject(uint32_t objectID);
    SPoint getTacticalPos() { return m_tacticalPos; }
    const PlayerClass& LocalPlayer() { return m_players.front(); }
    void Update(float dt);
    //FIXME: This shouldn't be here
    bool placingMode;

    //void Select(List* objectList);
    //void Unselect(List* objectList);
  protected:

    MapClass *m_map;
    std::vector<PlayerClass> m_players;
    ObjectTypeMap m_objects;
    uint32_t m_objectIDCounter;
    SPoint m_tacticalPos;
};

#endif // DUNE_GAMEMANAGER_H

