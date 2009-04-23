#include <eastwood/IniFile.h>

#include "MapGenerator.h"

#include "DuneConstants.h"
#include "GameMan.h"
#include "Log.h"
#include "MapClass.h"
#include "MapSeed.h"
#include "objects/ObjectClass.h"
#include "ResMan.h"

#include "mmath.h"

MapGenerator::MapGenerator ()
{
    //FIXME:All these should be tweakable during the game
    m_rockSpots = 40;//ROCKFIELDS;
    m_spiceFields = 4;//SPICEFIELDS;
    m_spiceBlooms = 10;
    m_rockBits = 10;
}

MapGenerator::~MapGenerator()
{

}

void MapGenerator::addRockBits()
{
    for(int done = 0; done < m_rockBits; done++)
    {
	SPoint spot(getRandom<short>(0, m_map->w - 1), getRandom<short>(0, m_map->h));
	TerrainClass *tmp = m_map->getCell(spot);

        if (tmp->getType() == Terrain_Sand)
        {
            tmp->setTile(Terrain_t10);      // Rock bit
            tmp->setType(Terrain_Rock);
        }

    }
}

void MapGenerator::addBlooms()
{
    for(int done = 0; done < m_spiceBlooms; done++)
    {
	SPoint spot(getRandom<short>(0, m_map->w - 1), getRandom<short>(0, m_map->h));
	TerrainClass *tmp = m_map->getCell(spot);

        if (tmp->getType() == Terrain_Sand && tmp->getTile() == Terrain_a1)
            tmp->setTile(getRandom<short>(Terrain_a2, Terrain_a3));     // Spice bloom

    } // WHILE
}

bool MapGenerator::checkCell(SPoint& cellPos)
{
    if ((cellPos.x < 0) || (cellPos.y < 0) || (!m_map->contains(cellPos)))
        return true;

    return false;
}

void MapGenerator::clearTerrain(int tile, int type)
{
    for (int i = 0; i < m_map->w; i++)
    {
        for (int j = 0; j < m_map->h; j++)
        {
            m_map->getCell(SPoint(i, j))->setType(type);
            m_map->getCell(SPoint(i, j))->setTile(tile);
        }
    }
}

bool MapGenerator::fixCell(SPoint& cellPos)
{
    bool error = false;

    if (cellPos.x < 0)
    {
        cellPos.x = 0;
        error = true;
    }

    else if (cellPos.x >= m_map->w)
    {
        cellPos.x = m_map->w - 1;
        error = true;
    }

    if (cellPos.y < 0)
    {
        cellPos.y = 0;
        error = true;
    }

    else if (cellPos.y >= m_map->w)
    {
        cellPos.y = m_map->w - 1;
        error = true;
    }

    return error;
}

MapClass* MapGenerator::createOldMap(std::string FieldString, int SeedNum, std::string BloomString)
{
    m_map = new MapClass(UPoint(64,64));
 
    unsigned short SeedMap[64*64];
    createMapWithSeed(SeedNum, SeedMap);


    for (int j = 0; j < m_map->h; j++)
        for (int i = 0; i < m_map->w; i++)
        {
            int type = Terrain_Sand;
            unsigned char seedmaptype = SeedMap[j*64+i] >> 4;
	    SPoint pos(i,j);

            switch (seedmaptype)
            {

                case 0x7:
                    /* Sand */
                    type = Terrain_Sand;
                    break;

                case 0x9:
                    /* Dunes */
                    type = Terrain_Dunes;
                    break;

                case 0x2:
                    /* Building */

                case 0x8:
                    /* Rock */
                    type = Terrain_Rock;
                    break;

                case 0xb:
                    /* Spice */
                    type = Terrain_Spice;
                    break;

                case 0xc:
                    /* ThickSpice */
                    type = Terrain_ThickSpice;
                    break;

                case 0xa:
                    /* Mountain */
                    type = Terrain_Mountain;
                    break;

                default:
                    LOG_ERROR("MapClass", "Unknown maptype %x\n", type);
                    exit(EXIT_FAILURE);
            }

            m_map->getCell(pos)->setType(type);
            m_map->getCell(pos)->setTile(Terrain_a1);
        }

    //createSandRegions();

    if (BloomString != "")
    {
        vector<std::string> BloomPositions  = SplitString(BloomString);

        for (unsigned int i = 0; i < BloomPositions.size();i++)
        {
            // set bloom
            int BloomPos = atol(BloomPositions[i].c_str());

            if ((BloomPos != 0) || (BloomPositions[i] == "0"))
            {
                SPoint pos(BloomPos % m_map->w, BloomPos / m_map->w);

                if (m_map->cellExists(pos))
                    m_map->getCell(pos)->setTile(getRandom<short>(Terrain_a2, Terrain_a3));
                else
                    LOG_WARNING("MapClass", "Cannot set bloom at %d, %d\n", pos.x, pos.y);
            }
        }
    }

    if (FieldString != "")
    {
        vector<std::string> FieldPositions  = SplitString(FieldString);

        for (unsigned int i = 0; i < FieldPositions.size();i++)
        {
            // set bloom
            int FieldPos = atol(FieldPositions[i].c_str());

            if ((FieldPos != 0) || (FieldPositions[i] == "0"))
            {
                SPoint pos(FieldPos % m_map->w, FieldPos / m_map->w);

                if (m_map->cellExists(pos))
                {
                    for (int x = -6; x <= 6; x++)
                        for (int y = -6; y <= 6; y++)
                            if (m_map->cellExists(pos + SPoint(x, y))
                                    && (distance_from(pos.x, pos.y, pos.x + x, pos.y + y) <= 6))
                            {
                                TerrainClass *cell = m_map->getCell(pos + SPoint(x, y));

                                if ((cell != NULL) & (cell->isSand()))
                                    cell->setType(Terrain_Spice);
                            }

                    for (int x = pos.x - 8; x <= pos.x + 8; x++)
                        for (int y = pos.y - 8; y <= pos.y + 8; y++)
                        {
                            if (m_map->cellExists(UPoint(x, y)))
                                smoothSpot(UPoint(x, y));
                        }
                }

                else
                {
                    LOG_WARNING("MapGenerator", "Cannot set field at %d, %d", pos.x, pos.y);
                }
            }
        }
    }

    smoothTerrain();
    
    return m_map;
}

bool MapGenerator::makeRandomMap(UPoint size)
{
    m_map = new MapClass(size);

    clearTerrain(Terrain_a1, Terrain_Sand);

    for (int i = 0; i < m_rockSpots; i++)
    {
	SPoint spot(getRandom(0, m_map->w - 1), getRandom(0, m_map->h - 1));
        makeSpot(spot, Terrain_Rock);
    }

    // Spice fields

    for (int i = 0; i < m_spiceFields; i++)
    {
	SPoint spot(getRandom(0, m_map->w - 1), getRandom(0, m_map->h - 1));
        makeSpot(spot, Terrain_Spice);
    }

    for (int count = 0; count < ROCKFILLER; count++)
        thickSpots(Terrain_Rock); //SPOT ROCK

    for (int count = 0; count < SPICEFILLER; count++)
        thickSpots(Terrain_Spice);

    addRockBits();

    addBlooms();

    smoothTerrain();

    return true;
}

void MapGenerator::makeSpot(SPoint cellPos, int type)
{
    int dir;  // Direction
    int j;   // Loop control

    for (j = 0; j < 1000; j++)
    {
        dir = getRandom(0, 3); // Random Dir

        switch (dir)
        {

            case 0 :
                cellPos.x--;
                break;

            case 1 :
                cellPos.x++;
                break;

            case 2 :
                cellPos.y--;
                break;

            case 3 :
                cellPos.y++;
                break;
        }

        fixCell(cellPos);

        if (type == Terrain_Spice)
        {
            if (m_map->getCell(cellPos)->getType() == Terrain_Rock)
                continue;  // Do not place the spice spot, priority is ROCK!
        }

        m_map->getCell(cellPos)->setTile(1);

        m_map->getCell(cellPos)->setType(type);
    }
}

int MapGenerator::on_left(SPoint cellPos, int tile)
{
    // Return 0 for NO, != 0 for YES
    cellPos.x--;
    fixCell(cellPos);

    if (m_map->getCell(cellPos)->getType() == tile)
        return 1;

    return 0;
}

int MapGenerator::on_right(SPoint cellPos, int tile)
{
    // Return 0 for NO, != 0 for YES
    cellPos.x++;
    fixCell(cellPos);

    if (m_map->getCell(cellPos)->getType() == tile)
        return 1;

    return 0;
}

int MapGenerator:: on_up(SPoint cellPos, int tile)
{
    // Return 0 for NO, != 0 for YES
    cellPos.y--;
    fixCell(cellPos);

    if (m_map->getCell(cellPos)->getType() == tile)
        return 1;

    return 0;
}

int MapGenerator:: on_down(SPoint cellPos, int tile)
{
    // Return 0 for NO, != 0 for YES
    cellPos.y++;
    fixCell(cellPos);

    if (m_map->getCell(cellPos)->getType() == tile)
        return 1;

    return 0;
}

int MapGenerator::side4(SPoint cellPos, int tile)
{
    // Check at 4 sides for 'tile'
    int flag = 0;

    if (on_left(cellPos, tile))
        flag++;

    if (on_right(cellPos, tile))
        flag++;

    if (on_up(cellPos, tile))
        flag++;

    if (on_down(cellPos, tile))
        flag++;

    return flag;
}

void MapGenerator::smoothSpot(SPoint spotPos)
{
    int left = 0;
    int right = 0;
    int up = 0;
    int down = 0;
    int maketile;
    TerrainClass * tmp = m_map->getCell(spotPos);


    if (tmp->getType() == Terrain_Dunes)
    {
        maketile = Terrain_DunesFull;                   // solid Dunes
        up = on_up(spotPos, Terrain_Dunes);
        down = on_down(spotPos, Terrain_Dunes);
        left = on_left(spotPos, Terrain_Dunes);
        right = on_right(spotPos, Terrain_Dunes);
        // Now perform the test

        if ((left == 1) && (right == 1) && (up == 1) && (down == 1))
            maketile = Terrain_DunesFull; //solid rock
        else if ((left == 0) && (right == 1) && (up == 1) && (down == 1))
            maketile = Terrain_DunesNotLeft; //missing left edge
        else if ((left == 1) && (right == 0) && (up == 1) && (down == 1))
            maketile = Terrain_DunesNotRight; //missing right edge
        else if ((left == 1) && (right == 1) && (up == 0) && (down == 1))
            maketile = Terrain_DunesNotUp; //missing top edge
        else if ((left == 1) && (right == 1) && (up == 1) && (down == 0))
            maketile = Terrain_DunesNotDown; //missing bottom edge
        else if ((left == 0) && (right == 1) && (up == 0) && (down == 1))
            maketile = Terrain_DunesDownRight; //missing top left edge
        else if ((left == 1) && (right == 0) && (up == 1) && (down == 0))
            maketile = Terrain_DunesUpLeft; //missing bottom right edge
        else if ((left == 1) && (right == 0) && (up == 0) && (down == 1))
            maketile = Terrain_DunesDownLeft; //missing top right edge
        else if ((left == 0) && (right == 1) && (up == 1) && (down == 0))
            maketile = Terrain_DunesUpRight; //missing bottom left edge
        else if ((left == 1) && (right == 1) && (up == 0) && (down == 0))
            maketile = Terrain_DunesLeftRight; //missing above and below
        else if ((left == 1) && (right == 0) && (up == 0) && (down == 0))
            maketile = Terrain_DunesLeft; //missing above, right and below
        else if ((left == 0) && (right == 1) && (up == 0) && (down == 0))
            maketile = Terrain_DunesRight; //missing above, left and below
        else if ((left == 0) && (right == 0) && (up == 1) && (down == 0))
            maketile = Terrain_DunesUp; //
        else if ((left == 0) && (right == 0) && (up == 0) && (down == 1))
            maketile = Terrain_DunesDown; //
        else if ((left == 0) && (right == 0) && (up == 1) && (down == 1))
            maketile = Terrain_DunesUpDown; //missing left and right
        else if ((left == 0) && (right == 0) && (up == 0) && (down == 0))
            maketile = Terrain_DunesIsland; //missing all edges

///////
        tmp->setTile(maketile);
    }

    else if (tmp->getType() == Terrain_Mountain)
    {
        maketile = Terrain_MountainFull;                   // solid mountain
        up = on_up(spotPos, Terrain_Mountain);
        down = on_down(spotPos, Terrain_Mountain);
        left = on_left(spotPos, Terrain_Mountain);
        right = on_right(spotPos, Terrain_Mountain);

        // Now perform the test

        if ((left == 1) && (right == 1) && (up == 1) && (down == 1))
            maketile = Terrain_MountainFull; //solid rock
        else if ((left == 0) && (right == 1) && (up == 1) && (down == 1))
            maketile = Terrain_MountainNotLeft; //missing left edge
        else if ((left == 1) && (right == 0) && (up == 1) && (down == 1))
            maketile = Terrain_MountainNotRight; //missing right edge
        else if ((left == 1) && (right == 1) && (up == 0) && (down == 1))
            maketile = Terrain_MountainNotUp; //missing top edge
        else if ((left == 1) && (right == 1) && (up == 1) && (down == 0))
            maketile = Terrain_MountainNotDown; //missing bottom edge
        else if ((left == 0) && (right == 1) && (up == 0) && (down == 1))
            maketile = Terrain_MountainDownRight; //missing top left edge
        else if ((left == 1) && (right == 0) && (up == 1) && (down == 0))
            maketile = Terrain_MountainUpLeft; //missing bottom right edge
        else if ((left == 1) && (right == 0) && (up == 0) && (down == 1))
            maketile = Terrain_MountainDownLeft; //missing top right edge
        else if ((left == 0) && (right == 1) && (up == 1) && (down == 0))
            maketile = Terrain_MountainUpRight; //missing bottom left edge
        else if ((left == 1) && (right == 1) && (up == 0) && (down == 0))
            maketile = Terrain_MountainLeftRight; //missing above and below
        else if ((left == 1) && (right == 0) && (up == 0) && (down == 0))
            maketile = Terrain_MountainLeft; //missing above, right and below
        else if ((left == 0) && (right == 1) && (up == 0) && (down == 0))
            maketile = Terrain_MountainRight; //missing above, left and below
        else if ((left == 0) && (right == 0) && (up == 1) && (down == 0))
            maketile = Terrain_MountainUp; //
        else if ((left == 0) && (right == 0) && (up == 0) && (down == 1))
            maketile = Terrain_MountainDown; //
        else if ((left == 0) && (right == 0) && (up == 1) && (down == 1))
            maketile = Terrain_MountainUpDown; //missing left and right
        else if ((left == 0) && (right == 0) && (up == 0) && (down == 0))
            maketile = Terrain_MountainIsland; //missing all edges

///////
        tmp->setTile(maketile);
    }

    else if (tmp->getType() == Terrain_Rock)
    {
        maketile = Terrain_t1;                   // Standard we have a solid rock plateau

        up = on_up(spotPos, Terrain_Rock) ||
             on_up(spotPos, Structure_Slab1) ||
             on_up(spotPos, Structure_Wall) ||
             on_up(spotPos, Terrain_Mountain);

        down = on_down(spotPos, Terrain_Rock) ||
               on_down(spotPos, Structure_Slab1) ||
               on_down(spotPos, Structure_Wall) ||
               on_down(spotPos, Terrain_Mountain);

        left = on_left(spotPos, Terrain_Rock) ||
               on_left(spotPos, Structure_Slab1) ||
               on_left(spotPos, Structure_Wall) ||
               on_left(spotPos, Terrain_Mountain);

        right = on_right(spotPos, Terrain_Rock) ||
                on_right(spotPos, Structure_Slab1) ||
                on_right(spotPos, Structure_Wall) ||
                on_right(spotPos, Terrain_Mountain);

        // Now perform the test

        if ((left == 1) && (right == 1) && (up == 1) && (down == 1))
            maketile = Terrain_t1; //solid rock
        else if ((left == 0) && (right == 1) && (up == 1) && (down == 1))
            maketile = Terrain_t2; //missing left edge
        else if ((left == 1) && (right == 0) && (up == 1) && (down == 1))
            maketile = Terrain_t3; //missing right edge
        else if ((left == 1) && (right == 1) && (up == 0) && (down == 1))
            maketile = Terrain_t4; //missing top edge
        else if ((left == 1) && (right == 1) && (up == 1) && (down == 0))
            maketile = Terrain_t5; //missing bottom edge
        else if ((left == 0) && (right == 1) && (up == 0) && (down == 1))
            maketile = Terrain_t6; //missing top left edge
        else if ((left == 1) && (right == 0) && (up == 1) && (down == 0))
            maketile = Terrain_t7; //missing bottom right edge
        else if ((left == 1) && (right == 0) && (up == 0) && (down == 1))
            maketile = Terrain_t8; //missing top right edge
        else if ((left == 0) && (right == 1) && (up == 1) && (down == 0))
            maketile = Terrain_t9; //missing bottom left edge
        else if ((left == 1) && (right == 1) && (up == 0) && (down == 0))
            maketile = Terrain_t11; //missing above and below
        else if ((left == 1) && (right == 0) && (up == 0) && (down == 0))
            maketile = Terrain_t12; //missing above, right and below
        else if ((left == 0) && (right == 1) && (up == 0) && (down == 0))
            maketile = Terrain_t13; //missing above, left and below
        else if ((left == 0) && (right == 0) && (up == 1) && (down == 0))
            maketile = Terrain_t14; //sand not up
        else if ((left == 0) && (right == 0) && (up == 0) && (down == 1))
            maketile = Terrain_t15; //sand not down
        else if ((left == 0) && (right == 0) && (up == 1) && (down == 1))
            maketile = Terrain_t16; //missing left and right
        else if ((left == 0) && (right == 0) && (up == 0) && (down == 0))
            maketile = Terrain_t10; //missing all edges

        ///////
        tmp->setTile(maketile);
    }

    else if (tmp->getType() == Terrain_Spice)
    {
        maketile = Terrain_s1;                  // Standard we have a solid spice
        up = on_up(spotPos, Terrain_Spice) ||
             on_up(spotPos, Terrain_ThickSpice);

        down = on_down(spotPos, Terrain_Spice) ||
               on_down(spotPos, Terrain_ThickSpice);

        left = on_left(spotPos, Terrain_Spice) ||
               on_left(spotPos, Terrain_ThickSpice);

        right = on_right(spotPos, Terrain_Spice) ||
                on_right(spotPos, Terrain_ThickSpice);

        // Now perform the test

        if ((left == 1) && (right == 1) && (up == 1) && (down == 1))
            maketile = Terrain_s1; //solid spice
        else if ((left == 0) && (right == 1) && (up == 1) && (down == 1))
            maketile = Terrain_s2; //missing left edge
        else if ((left == 1) && (right == 0) && (up == 1) && (down == 1))
            maketile = Terrain_s3; //missing right edge
        else if ((left == 1) && (right == 1) && (up == 0) && (down == 1))
            maketile = Terrain_s4; //missing top edge
        else if ((left == 1) && (right == 1) && (up == 1) && (down == 0))
            maketile = Terrain_s5; //missing bottom edge
        else if ((left == 0) && (right == 1) && (up == 0) && (down == 1))
            maketile = Terrain_s6; //missing top left edge
        else if ((left == 1) && (right == 0) && (up == 1) && (down == 0))
            maketile = Terrain_s7; //missing bottom right edge
        else if ((left == 1) && (right == 0) && (up == 0) && (down == 1))
            maketile = Terrain_s8; //missing top right edge
        else if ((left == 0) && (right == 1) && (up == 1) && (down == 0))
            maketile = Terrain_s9; //missing bottom left edge
        else if ((left == 1) && (right == 1) && (up == 0) && (down == 0))
            maketile = Terrain_s11; //missing above and below
        else if ((left == 1) && (right == 0) && (up == 0) && (down == 0))
            maketile = Terrain_s12; //missing above, right and below
        else if ((left == 0) && (right == 1) && (up == 0) && (down == 0))
            maketile = Terrain_s13; //missing above, left and below
        else if ((left == 0) && (right == 0) && (up == 1) && (down == 0))
            maketile = Terrain_s14; //sand not up
        else if ((left == 0) && (right == 0) && (up == 0) && (down == 1))
            maketile = Terrain_s15; //sand not down
        else if ((left == 0) && (right == 0) && (up == 1) && (down == 1))
            maketile = Terrain_s16; //missing left and right
        else if ((left == 0) && (right == 0) && (up == 0) && (down == 0))
            maketile = Terrain_s10; //no edges

        ///////
        tmp->setTile(maketile);
    }

    else if (tmp->getType() == Terrain_ThickSpice)
    {
        maketile = Terrain_ThickSpiceFull;                   // solid ThickSpice
        up = on_up(spotPos, Terrain_ThickSpice);
        down = on_down(spotPos, Terrain_ThickSpice);
        left = on_left(spotPos, Terrain_ThickSpice);
        right = on_right(spotPos, Terrain_ThickSpice);

        // Now perform the test

        if ((left == 1) && (right == 1) && (up == 1) && (down == 1))
            maketile = Terrain_ThickSpiceFull; //solid rock
        else if ((left == 0) && (right == 1) && (up == 1) && (down == 1))
            maketile = Terrain_ThickSpiceNotLeft; //missing left edge
        else if ((left == 1) && (right == 0) && (up == 1) && (down == 1))
            maketile = Terrain_ThickSpiceNotRight; //missing right edge
        else if ((left == 1) && (right == 1) && (up == 0) && (down == 1))
            maketile = Terrain_ThickSpiceNotUp; //missing top edge
        else if ((left == 1) && (right == 1) && (up == 1) && (down == 0))
            maketile = Terrain_ThickSpiceNotDown; //missing bottom edge
        else if ((left == 0) && (right == 1) && (up == 0) && (down == 1))
            maketile = Terrain_ThickSpiceDownRight; //missing top left edge
        else if ((left == 1) && (right == 0) && (up == 1) && (down == 0))
            maketile = Terrain_ThickSpiceUpLeft; //missing bottom right edge
        else if ((left == 1) && (right == 0) && (up == 0) && (down == 1))
            maketile = Terrain_ThickSpiceDownLeft; //missing top right edge
        else if ((left == 0) && (right == 1) && (up == 1) && (down == 0))
            maketile = Terrain_ThickSpiceUpRight; //missing bottom left edge
        else if ((left == 1) && (right == 1) && (up == 0) && (down == 0))
            maketile = Terrain_ThickSpiceLeftRight; //missing above and below
        else if ((left == 1) && (right == 0) && (up == 0) && (down == 0))
            maketile = Terrain_ThickSpiceLeft; //missing above, right and below
        else if ((left == 0) && (right == 1) && (up == 0) && (down == 0))
            maketile = Terrain_ThickSpiceRight; //missing above, left and below
        else if ((left == 0) && (right == 0) && (up == 1) && (down == 0))
            maketile = Terrain_ThickSpiceUp; //
        else if ((left == 0) && (right == 0) && (up == 0) && (down == 1))
            maketile = Terrain_ThickSpiceDown; //
        else if ((left == 0) && (right == 0) && (up == 1) && (down == 1))
            maketile = Terrain_ThickSpiceUpDown; //missing left and right
        else if ((left == 0) && (right == 0) && (up == 0) && (down == 0))
            maketile = Terrain_ThickSpiceIsland; //missing all edges

        ///////
        tmp->setTile(maketile);
    }

    else if (tmp->getType() == Terrain_Sand)
    {
        if ((tmp->getTile() != Terrain_a2) && (tmp->getType() != Terrain_a3))
        {
            tmp->setTile(Terrain_a1);
        }
    }
}

void MapGenerator::smoothTerrain()
{
    int i, j;

    // Smooth the darn terrain

    for (i = 0; i < m_map->w; i++)
        for (j = 0; j < m_map->h; j++)
            smoothSpot(SPoint(i, j));
}

void MapGenerator::thickSpots(int type)  //removes holes in rock and spice
{
    for (int i = 0; i < m_map->w; i++)
        for (int j = 0; j < m_map->h; j++)
        {
            if (m_map->getCell(SPoint(i, j))->getType() != type)                 // Found something else than what thickining
            {
                if (side4(SPoint(i, j), type) >= 3)
                    m_map->getCell(SPoint(i, j))->setType(type);               // Seems enough of the type around it so make this rock

                if (side4(SPoint(i, j), type) == 2)                // Gamble, fifty fifty... rock or not?
                {
                    if (getRandom(0, 1) == 1)
                        m_map->getCell(SPoint(i, j))->setType(type);
                }
            }
        }
}

vector<std::string> MapGenerator::SplitString(std::string ParseString)
{
    vector<std::string> retVector;
    int startpos = 0;

    for (unsigned int i = 0; i < ParseString.size(); i++)
    {
        if (ParseString[i] == ',')
        {
            retVector.push_back(ParseString.substr(startpos, i - startpos));
            startpos = i + 1;
        }
    }

    retVector.push_back(ParseString.substr(startpos, ParseString.size() - startpos));

    return retVector;
}

#if 0
int readMapPlayerNumbers(char* filename, int numbers[MAX_PLAYERS])
{
    int maxPlayers = NONE;
    FILE *file = fopen(filename, "rb");

    if (!file)
        fprintf(stderr, "Could not open m_map file %s.\n", filename);
    else
    {
        int magicNum;

        fread(&magicNum, sizeof(int), 1, file);

        if (magicNum == MAPMAGIC)
        {
            fread(&maxPlayers, sizeof(int), 1, file);

            if ((maxPlayers >= 0) && (maxPlayers <= MAX_PLAYERS))
            {
                //read the players details
                for (int i = 0; i < maxPlayers; i++)
                    fread(&numbers[i], sizeof(int), 1, file);
            }
        }

        fclose(file);
    }

    return maxPlayers;
}

int readMapMaxPlayers(char* filename)
{
    int maxPlayers = 0;
    FILE *file = fopen(filename, "rb");

    if (!file)
        fprintf(stderr, "Could not open m_map file %s.\n", filename);
    else
    {
        int  magicNum;

        fread(&magicNum, sizeof(int), 1, file);

        if (magicNum == MAPMAGIC)
        {
            fread(&maxPlayers, sizeof(int), 1, file);

            if ((maxPlayers <= 0) || (maxPlayers > MAX_PLAYERS))
                maxPlayers = 0;
        }

        fclose(file);
    }

    return maxPlayers;
}


bool loadNewMap(FILE* file)
{
    bool found[MAX_PLAYERS],
    ok = false;

    short mapSizeX, mapSizeY;
    int  i, j, x, y, itemID, num,
    playerNum, maxPlayers, givenPlayer;
    UnitClass*  newUnit;
    StructureClass* newStructure;

    fread(&maxPlayers, sizeof(int), 1, file);

    for (i = 0; i < maxPlayers; i++)
        found[i] = false;

    //read the players details
    for (i = 0; i < maxPlayers; i++)
    {
        fread(&j, sizeof(int), 1, file);

        if ((j >= 0) && (j < MAX_PLAYERS))
            found[j] = true;
        else
            return false;
    }

    fread(&mapSizeX, sizeof(short), 1, file);

    fread(&mapSizeY, sizeof(short), 1, file);

    if ((mapSizeX >= 0) && (mapSizeX < MAX_XSIZE) && (mapSizeY >= 0) && (mapSizeY < MAX_YSIZE))
        ok = true;
    else
        fprintf(stderr, "Level size is unsupported %d,%d.", mapSizeX, mapSizeY);

    if (ok)
    {
        m_map = new MapClass(mapSizeX, mapSizeY);

        //load terrain data

        for (i = 0; i < m_map->sizeX; i++)
            for (j = 0; j < m_map->sizeY; j++)
            {
                fread(&x, sizeof(int), 1, file);
                m_map->getCell(i, j)->setType(x);
                fread(&x, sizeof(int), 1, file);
                m_map->getCell(i, j)->setTile(x);
            }

        m_map->createSandRegions();

        smooth_terrain();

        //load the structure info
        fread(&num, sizeof(int), 1, file);

        for (i = 0; i < num; i++)
        {
            fread(&playerNum, sizeof(int), 1, file);
            fread(&itemID, sizeof(int), 1, file);
            fread(&x, sizeof(int), 1, file);
            fread(&y, sizeof(int), 1, file);

            //find the player associated to this m_map player

            for (givenPlayer = 0; givenPlayer < currentGame->maxPlayers; givenPlayer++)
                if ((currentGame->player[givenPlayer] != NULL) && (currentGame->player[givenPlayer]->getMapPlayerNum() == playerNum))
                    break; //found it

            if (givenPlayer < currentGame->maxPlayers)
            { //found player to give to
                newStructure = (StructureClass*) (currentGame->player[givenPlayer]->placeStructure(NONE, NONE, NONE, itemID, x, y));

                if ((itemID == Structure_GunTurret)
                        || (itemID == Structure_RocketTurret))
                {
                    fread(&x, sizeof(int), 1, file);
                    ((GunTurretClass*)newStructure)->setTurretAngle(x);
                }
            } //don't make the structure because no owner associated

            else if ((itemID == Structure_GunTurret)
                     || (itemID == Structure_RocketTurret))
                fread(&x, sizeof(int), 1, file); //still read all of structure info
        }

        //load the unit info
        fread(&num, sizeof(int), 1, file);

        for (i = 0; i < num; i++)
        {
            fread(&playerNum, sizeof(int), 1, file);
            fread(&itemID, sizeof(int), 1, file);
            fread(&x, sizeof(int), 1, file);
            fread(&y, sizeof(int), 1, file);

            //find the player associated to this m_map player

            for (givenPlayer = 0; givenPlayer < currentGame->maxPlayers; givenPlayer++)
                if ((currentGame->player[givenPlayer] != NULL) && (currentGame->player[givenPlayer]->getMapPlayerNum() == playerNum))
                    break; //found it

            if (givenPlayer < currentGame->maxPlayers)
            { //found player to give to
                newUnit = ((UnitClass*) (currentGame->player[givenPlayer]->placeUnit(itemID, x, y)));
                fread(&x, sizeof(int), 1, file);

                if (newUnit)
                {
                    newUnit->setAngle(x);

                    if (newUnit->isTurreted())
                    {
                        fread(&x, sizeof(int), 1, file);
                        ((TankClass*)newUnit)->setTurretAngle(x);
                    }
                }
            }

            else
            {
                fread(&x, sizeof(int), 1, file); //still read all of unit info

                if ((itemID == Unit_Tank) || (itemID == Unit_SiegeTank)) //nasty hack - FIXME
                    fread(&x, sizeof(int), 1, file);
            }
        }

        m_map->setWinFlags(3);
    }

    return ok;
}


bool loadMap(char* mapName)
{
    bool ok = false;
    FILE *mapFile = fopen(mapName, "rb");

    if (!mapFile)
    {
        fprintf(stderr, "Could not open m_map file %s.\n", mapName);
        exit(EXIT_FAILURE);
    }

    int  magicNum;

    fread(&magicNum, sizeof(int), 1, mapFile);

    ok = loadNewMap(mapFile);
    fclose(mapFile);

    if (!structureList->isEmpty())
    {
        COORDTYPE* pos;
        StructureClass *tempStructure;
        structureList->reset();

        while (structureList->currentNotNull())
        {
            tempStructure = (StructureClass*)structureList->getCurrent();

            if (tempStructure->getItemID() == Structure_Refinery)
            {
                pos = tempStructure->getLocation();

                if (gameState != GS_NONE)
                    tempStructure->getOwner()->freeHarvester(pos->x, pos->y);
            }

            structureList->nextLink();
        }
    }

    return ok;
}

#endif // 0
