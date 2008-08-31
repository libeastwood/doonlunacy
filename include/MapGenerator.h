#ifndef DUNE_MAPGENERATOR_H
#define DUNE_MAPGENERATOR_H

#include "Gfx.h"
#include "singleton.h"
#include "ObjectClass.h"
#include "MapClass.h"
#include "PlayerClass.h"

class GameMan;

/*!
 *  Singleton class used to generate or load Map files.
 *  It can load both old school dune maps and new type binary maps created in
 *  EditorClass which is still to come
 *
 */

class MapGenerator : public Singleton<MapGenerator>
{

    friend class Singleton<MapGenerator>;

  protected:
    //! @name Constructor and destructor
    //@{
    MapGenerator();
    ~MapGenerator();
    //@}

  public:
    //! @name Map loading and generating
    //@{
    void addPlayer(PLAYERHOUSE House, bool ai, int team);

    GameMan* getGMan() { return m_gman; }

    //! Creates a random map
    /*!
     *  @return true if map was successfully generated, false if something went wrong
     *  @note TODO: it should use some variables. e.g. number of players, spice/rock density
     */
    bool makeRandomMap(UPoint size);

    //No idea which one actually loads the map. I'll get random map running first.
    bool loadMap(std::string mapName);
    bool loadNewMap(std::string fileName);
    MapClass* createOldMap(std::string FieldString, int SeedNum, std::string BloomString);
    //@}

    //! @name Auxilliary functions
    //@{
    /*!
     * Fixes coordinates of cell. Sometimes coordinates can get outside the map's size.
     * This prevents bad things from happening.
     */
    bool checkCell(SPoint& cellPos);
    
    bool fixCell(SPoint& cellPos);
    void fixWalls(SPoint pos);

    //@}

  private:

    void addRockBits();

    //! Adds spice blooms. This cute bubbles that explode when you step on them
    void addBlooms();

    void clearTerrain(int tile, int type);
    void fixWall(SPoint pos);

    //! Function used to create spice fields and rock areas
    void makeSpot(SPoint cellPos, int type);

    //! Return 1 (YES) if type of tile to the left is equal to tile, and NO(0) if it isn't
    int on_left(SPoint cellPos, int tile);

    //! Return 1 (YES) if type of tile to the right is equal to tile, and NO(0) if it isn't
    int on_right(SPoint cellPos, int tile);

    //! Return 1 (YES) if type of tile to the top is equal to tile, and NO(0) if it isn't
    int on_up(SPoint cellPos, int tile);

    //! Return 1 (YES) if type of tile to the bottom is equal to tile, and NO(0) if it isn't
    int on_down(SPoint cellPos, int tile);

    // Check at 4 sides for 'tile' (up, down, left, right)
    int side4(SPoint cellPos, int tile);

    void smoothSpot(SPoint pos);

    void smoothTerrain();

    //!removes holes in rock and spice
    void thickSpots(int tpe);

    std::vector<std::string> SplitString(std::string ParseString);
    bool SplitString(std::string ParseString, unsigned int NumStringPointers, ...);

    //FIXME: What's the difference between all these things?:)
    //! Amount of spice blooms on a randomly generated map
    int m_spiceBlooms;

    //! Amount of rock bits on a randomly generated map
    int m_rockBits;

    //! Amount of rock spots on a randomly generated map
    int m_rockSpots;

    //! Amount of spice fields on a randomly generated map
    int m_spiceFields;

    MapClass* m_map;

    //! Pointer to current game manager
    GameMan* m_gman;
};

#endif // DUNE_MAPGENERATOR_H
