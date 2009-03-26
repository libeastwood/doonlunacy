#ifndef DUNE_MAPCLASS_H
#define DUNE_MAPCLASS_H

#include <vector>

#include "Definitions.h"
#include "Gfx.h"
#include "Log.h"
#include "TerrainClass.h"

class MapGenerator;
class GameMan;

typedef std::vector<TerrainClass*> Cells;

class MapClass : public Rect
{
    friend class MapGenerator;

    public:
    //! @name Constructors & Destructor
    //@{
    /*!
     *  @param size UPoint representing size of the map in cells UPoint(64,64)) means that size is 64x64 cells, and 1024x1024pixels
     */
    MapClass(UPoint size);
    ~MapClass();
    //@}
    bool cellExists(SPoint pos);
    bool cellExists(int x, int y) { return cellExists(SPoint(x,y)); }


    //! @name position and angle calculation
    //@{

    /*!
     *  Calculates the angle from source point to other point
     *  @param source source point
     *  @param pos target point
     *  @return 0 means UP, and subsequent directions change clockwise
     */
    int getPosAngle(UPoint source, UPoint pos);

    /*!
     *  Calculates the position of a cell in a certain direction from the source point
     *  @param angle from source point to seeked point
     *  @param source source point
     *  @return coordinates of a cell
     */    
    UPoint getMapPos(int angle, UPoint source);
    //@}
    void damage(ObjectClass* damager, PlayerClass* damagerOwner, UPoint realPos, 
	    std::string objectName, int damage, int damagePiercing, int damageRadius, bool air);

    void fixWall(SPoint pos);
    void fixWalls(SPoint pos);
    void removeObjectFromMap(Uint32 ObjectID);
    void selectObjects(int playerNum, int x1, int y1, int x2, int y2, int realX, int realY, bool objectARGMode);
    void viewMap(int playerTeam, UPoint location, int maxViewRange);
    void viewMap(int playerTeam, int x, int y, int maxViewRange);

    ObjectClass* findObjectWithID(int objectID, int lx, int ly);

    TerrainClass * getCell(SPoint pos);    



    //FIXME:Remove this later or sth.
    short *depthCheckCount,
	  ***depthCheckMax;



    BLOCKEDTYPE cellBlocked(UPoint pos);

    protected:
    void createSandRegions();

    Cells m_cells;

    private: 
    ObjectClass* lastSinglySelectedObject;
    inline bool checkPos(SPoint pos, SPoint possy) {
	SPoint tmpPos = pos + possy;
	ObjectClass *tmpObj;

	return (!cellExists(tmpPos) || ((tmpObj = getCell(tmpPos)->getGroundObject()) != NULL
		    && tmpObj->getObjectName() == "Wall"));
	//|| (cell[i][j-1].getGroundObject()->getItemID() == Structure_GunTurret)
	//|| (cell[i][j-1].getGroundObject()->getItemID() == Structure_RocketTurret))));
    }

};

#endif // DUNE_MAPCLASS_H

