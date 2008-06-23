#include "DataCache.h" //It should not be included here, but all enums are here
#include "DuneConstants.h"
#include "GameState.h"
#include "Log.h"
#include "MapClass.h"
#include "MapGenerator.h"
#include "MapSeed.h"
#include "ObjectClass.h"

#include "pakfile/Inifile.h"

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

bool MapGenerator::loadOldMap(std::string mapName)
{
    bool done = false; //this will be set to false if any errors, so level won't load

    Inifile * myInifile = new Inifile(mapName);

    m_gs = new GameState();
    m_map = new MapClass(UPoint(64, 64));
    m_gs->m_map = m_map;

    int SeedNum = myInifile->getIntValue("MAP", "Seed", -1);

    if (SeedNum == -1)
    {
        LOG_ERROR("MapGenerator", "Cannot find Seednum in %s!", mapName.c_str());
        delete m_map;
        return false;
    }

    Players *m_players = m_gs->m_players;

    unsigned short SeedMap[64*64];
    createMapWithSeed(SeedNum, SeedMap);


    for (int j = 0; j < m_map->h; j++)
    {
        for (int i = 0; i < m_map->w; i++)
        {
            int type = Terrain_Sand;
            unsigned char seedmaptype = SeedMap[j*64+i] >> 4;

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
                    LOG_WARNING("MapClass", "Unknown maptype %x\n", type);
                    exit(EXIT_FAILURE);
            }

            m_map->getCell(i, j)->setType(type);

            m_map->getCell(i, j)->setTile(Terrain_a1);
        }

    }

    done = true;

    //createSandRegions();

    string BloomString = myInifile->getStringValue("MAP", "Bloom");

    if (BloomString != "")
    {
        vector<string> BloomPositions  = SplitString(BloomString);

        for (unsigned int i = 0; i < BloomPositions.size();i++)
        {
            // set bloom
            int BloomPos = atol(BloomPositions[i].c_str());

            if ((BloomPos != 0) || (BloomPositions[i] == "0"))
            {
                int xpos = BloomPos % m_map->w;
                int ypos = BloomPos / m_map->w;

                if (m_map->cellExists(xpos, ypos))
                {
                    m_map->getCell(xpos, ypos)->setTile(getRandomInt(Terrain_a2, Terrain_a3));
                }

                else
                {
                    LOG_WARNING("MapClass", "Cannot set bloom at %d, %d\n", xpos, ypos);
                }
            }
        }
    }

    string FieldString = myInifile->getStringValue("MAP", "Field");

    if (FieldString != "")
    {
        vector<string> FieldPositions  = SplitString(FieldString);

        for (unsigned int i = 0; i < FieldPositions.size();i++)
        {
            // set bloom
            int FieldPos = atol(FieldPositions[i].c_str());

            if ((FieldPos != 0) || (FieldPositions[i] == "0"))
            {
                int xpos = FieldPos % m_map->w;
                int ypos = FieldPos / m_map->w;

                if (m_map->cellExists(xpos, ypos))
                {
                    for (int x = -6; x <= 6; x++)
                    {
                        for (int y = -6; y <= 6; y++)
                        {
                            if (m_map->cellExists(xpos + x, ypos + y)
                                    && (distance_from(xpos, ypos, xpos + x, ypos + y) <= 6))
                            {
                                TerrainClass *cell = m_map->getCell(xpos + x, ypos + y);

                                if ((cell != NULL) & (cell->isSand()))
                                    cell->setType(Terrain_Spice);
                            }
                        }
                    }

                    for (int x = xpos - 8; x <= xpos + 8; x++)
                    {
                        for (int y = ypos - 8; y <= ypos + 8; y++)
                        {
                            if (m_map->cellExists(x, y))
                            {
                                smoothSpot(UPoint(x, y));
                            }
                        }
                    }
                }

                else
                {
                    LOG_WARNING("MapGenerator", "Cannot set field at %d, %d", xpos, ypos);
                }
            }
        }
    }

    smoothTerrain();

    // now set up all the players

    addPlayer(ATREIDES, false, 1);
    addPlayer(ORDOS, false, 1);
    addPlayer(HARKONNEN, false, 1);
    addPlayer(SARDAUKAR, false, 2);
    addPlayer(FREMEN, false, 2);
    addPlayer(MERCENERY, false, 2);

    Inifile::KeyListHandle myListHandle;

    myListHandle = myInifile->KeyList_Open("UNITS");

    while (!myInifile->KeyList_EOF(myListHandle))
    {
        string tmpkey = myInifile->KeyList_GetNextKey(&myListHandle);
        string tmp = myInifile->getStringValue("UNITS", tmpkey);
        string HouseStr, UnitStr, health, PosStr, rotation, mode;
        SplitString(tmp, 6, &HouseStr, &UnitStr, &health, &PosStr, &rotation, &mode);

        int house;
        int unitID;

        if ((HouseStr == "Atreides") || (HouseStr == "ATREIDES"))
            house = HOUSE_ATREIDES;
        else if ((HouseStr == "Ordos") || (HouseStr == "ORDOS"))
            house = HOUSE_ORDOS;
        else if ((HouseStr == "Harkonnen") || (HouseStr == "HARKONNEN"))
            house = HOUSE_HARKONNEN;
        else if ((HouseStr == "Fremen") || (HouseStr == "FREMEN"))
            house = HOUSE_FREMEN;
        else if ((HouseStr == "Sardaukar") || (HouseStr == "SARDAUKAR"))
            house = HOUSE_SARDAUKAR;
        else if ((HouseStr == "Mercenary") || (HouseStr == "MERCENARY"))
            house = HOUSE_MERCENARY;
        else
        {
            LOG_WARNING("MapClass", "Invalid house string: %s", HouseStr.c_str());
            house = HOUSE_ATREIDES;
        }

        int pos = atol(PosStr.c_str());

        if (pos <= 0)
        {
            LOG_WARNING("MapClass", "Invalid position string: %s", PosStr.c_str());
            pos = 0;
        }

        int Num2Place = 1;

        if (UnitStr == "Devastator")
            unitID = Unit_Devastator;
        else if (UnitStr == "Deviator")
            unitID = Unit_Deviator;
        else if (UnitStr == "Harvester")
            unitID = Unit_Harvester;
        else if (UnitStr == "Infantry")
        {
            //make three
            unitID = Unit_Infantry;
            Num2Place = 3;
        }

        else if (UnitStr == "Launcher")
            unitID = Unit_Launcher;
        else if (UnitStr == "MCV")
            unitID = Unit_MCV;
        else if (UnitStr == "Quad")
            unitID = Unit_Quad;
        else if (UnitStr == "Sandworm")
            unitID = Unit_Sandworm;
        else if (UnitStr == "Siege Tank")
            unitID = Unit_SiegeTank;
        else if (UnitStr == "Sonic Tank")
            unitID = Unit_SonicTank;
        else if (UnitStr == "Soldier")
            unitID = Unit_Infantry;
        else if (UnitStr == "Tank")
            unitID = Unit_Tank;
        else if (UnitStr == "Trike")
            unitID = Unit_Trike;
        else if (UnitStr == "Raider Trike")
            unitID = Unit_Raider;
        else if (UnitStr == "Raider")
            unitID = Unit_Raider;
        else if (UnitStr == "Troopers")
        {
            //make three
            unitID = Unit_Trooper;
            Num2Place = 3;
        }

        else if (UnitStr == "Trooper")
            unitID = Unit_Trooper;
        else
        {
            LOG_WARNING("MapGenerator", "Invalid unit string: %s", UnitStr.c_str());
            unitID = Unit_Quad;
        }

        /*        //FIXME: Fix this here and in addPlayer
          if(m_players->size() > house) {
           LOG_ERROR("MapGenerator", "player[%d]== NULL",(int) house);
           exit(EXIT_FAILURE);
          }
        */

        for (int i = 0; i < Num2Place; i++)
        {
            ObjectClass* newUnit = (ObjectClass*)m_players->at(house)->placeUnit(unitID, UPoint(pos % 64, pos / 64));

            if (newUnit == NULL)
            {
                LOG_WARNING("MapGenerator", "This file is not a valid unit entry: %d. (invalid unit position)", pos);
            }
        }
    }

    myInifile->KeyList_Close(&myListHandle);


    myListHandle = myInifile->KeyList_Open("STRUCTURES");

    while (!myInifile->KeyList_EOF(myListHandle))
    {
        string tmpkey = myInifile->KeyList_GetNextKey(&myListHandle);
        string tmp = myInifile->getStringValue("STRUCTURES", tmpkey);

        if (tmpkey.find("GEN") == 0)
        {
            // Gen Object/Structure
            string PosStr = tmpkey.substr(3, tmpkey.size() - 3);
            int pos = atol(PosStr.c_str());

            string HouseStr, BuildingStr;
            SplitString(tmp, 2, &HouseStr, &BuildingStr);

            int house;

            if ((HouseStr == "Atreides") || (HouseStr == "ATREIDES"))
                house = HOUSE_ATREIDES;
            else if ((HouseStr == "Ordos") || (HouseStr == "ORDOS"))
                house = HOUSE_ORDOS;
            else if ((HouseStr == "Harkonnen") || (HouseStr == "HARKONNEN"))
                house = HOUSE_HARKONNEN;
            else if ((HouseStr == "Fremen") || (HouseStr == "FREMEN"))
                house = HOUSE_FREMEN;
            else if ((HouseStr == "Sardaukar") || (HouseStr == "SARDAUKAR"))
                house = HOUSE_SARDAUKAR;
            else if ((HouseStr == "Mercenary") || (HouseStr == "MERCENARY"))
                house = HOUSE_MERCENARY;
            else
            {
                LOG_WARNING("MapGenerator", "loadOldMap: Invalid house string: %s", HouseStr.c_str());
                house = HOUSE_ATREIDES;
            }

            /*
                        //FIXME: Fix this here and in addPlayer
                  if(m_players->size() > house) {
                LOG_ERROR("MapGenerator","player[%d]== NULL",(int) house);
                exit(EXIT_FAILURE);
               }
            */

            if (BuildingStr == "Concrete")
            {
                m_players->at(house)->placeStructure(NONE, NONE, Structure_Slab1, UPoint(pos % 64, pos / 64));
            }

            else if (BuildingStr == "Wall")
            {
                m_players->at(house)->placeStructure(NONE, NONE, Structure_Wall, UPoint(pos % 64, pos / 64));
            }

            else
            {
                LOG_WARNING("MapGenerator", "loadINIMap: Invalid building string: %s", BuildingStr.c_str());
            }
        }

        else
        {
            // other structure
            string HouseStr, BuildingStr, health, PosStr;
            SplitString(tmp, 6, &HouseStr, &BuildingStr, &health, &PosStr);

            int pos = atol(PosStr.c_str());

            int house;

            if ((HouseStr == "Atreides") || (HouseStr == "ATREIDES"))
                house = HOUSE_ATREIDES;
            else if ((HouseStr == "Ordos") || (HouseStr == "ORDOS"))
                house = HOUSE_ORDOS;
            else if ((HouseStr == "Harkonnen") || (HouseStr == "HARKONNEN"))
                house = HOUSE_HARKONNEN;
            else if ((HouseStr == "Fremen") || (HouseStr == "FREMEN"))
                house = HOUSE_FREMEN;
            else if ((HouseStr == "Sardaukar") || (HouseStr == "SARDAUKAR"))
                house = HOUSE_SARDAUKAR;
            else if ((HouseStr == "Mercenary") || (HouseStr == "MERCENARY"))
                house = HOUSE_MERCENARY;
            else
            {
                LOG_WARNING("MapGenerator", "loadINIMap: Invalid house string: %s", HouseStr.c_str());
                house = HOUSE_ATREIDES;
            }

            int itemID = 0;

            if (BuildingStr == "Barracks")
                itemID = Structure_WOR;//Structure_Barracks;
            else if (BuildingStr == "Const Yard")
                itemID = Structure_ConstructionYard;
            else if (BuildingStr == "R-Turret")
                itemID = Structure_RocketTurret;
            else if (BuildingStr == "Turret")
                itemID = Structure_GunTurret;
            else if (BuildingStr == "Heavy Fctry")
                itemID = Structure_HeavyFactory;
            else if (BuildingStr == "Hi-Tech")
                itemID = Structure_HighTechFactory;
            else if (BuildingStr == "IX")
                itemID = Structure_IX;
            else if (BuildingStr == "Light Fctry")
                itemID = Structure_LightFactory;
            else if (BuildingStr == "Palace")
                itemID = Structure_Palace;
            else if (BuildingStr == "Outpost")
                itemID = Structure_Radar;
            else if (BuildingStr == "Refinery")
                itemID = Structure_Refinery;
            else if (BuildingStr == "Repair")
                itemID = Structure_RepairYard;
            else if (BuildingStr == "Spice Silo")
                itemID = Structure_Silo;
            else if (BuildingStr == "Star Port")
                itemID = Structure_StarPort;
            else if (BuildingStr == "Windtrap")
                itemID = Structure_WindTrap;
            else if (BuildingStr == "WOR")
                itemID = Structure_WOR;
            else
            {
                LOG_WARNING("MapGenerator", "loadINIMap: Invalid building: %s", BuildingStr.c_str());
                itemID = 0;
            }

            if ((m_players->at(house) != NULL) && (itemID != 0))
            {
                ObjectClass*  newStructure = (ObjectClass*)m_players->at(house)->placeStructure(NONE, NONE, itemID, UPoint(pos % 64, pos / 64));

                if (newStructure == NULL)
                {
                    LOG_WARNING("MapGenerator", "loadINIMap: Invalid position: %s", PosStr.c_str());
                }
            }

        }
    }

    myInifile->KeyList_Close(&myListHandle);

    return true;
}

/*
 Splits a string into several substrings. This strings are separated with ','.
*/
bool MapGenerator::SplitString(string ParseString, unsigned int NumStringPointers, ...)
{
    va_list arg_ptr;
    va_start(arg_ptr, NumStringPointers);

    string** pStr;

    if (NumStringPointers == 0)
        return false;

    if ((pStr = (string**) malloc(sizeof(string*) * NumStringPointers)) == NULL)
    {
        LOG_ERROR("MapClass", "SplitString: Cannot allocate memory!\n");
        exit(EXIT_FAILURE);
    }

    for (unsigned int i = 0; i < NumStringPointers; i++)
    {
        pStr[i] = va_arg(arg_ptr, string* );
    }

    va_end(arg_ptr);

    int startpos = 0;
    unsigned int index = 0;

    for (unsigned int i = 0; i < ParseString.size(); i++)
    {
        if (ParseString[i] == ',')
        {
            *(pStr[index]) = ParseString.substr(startpos, i - startpos);
            startpos = i + 1;
            index++;

            if (index >= NumStringPointers)
            {
                free(pStr);
                return false;
            }
        }
    }

    *(pStr[index]) = ParseString.substr(startpos, ParseString.size() - startpos);

    free(pStr);
    return true;
}


bool MapGenerator::makeRandomMap(UPoint size)
{
    int i, count;
    int spotX, spotY;

    m_map = new MapClass(size);

    clearTerrain(Terrain_a1, Terrain_Sand);

    for (i = 0; i < m_rockSpots; i++)
    {
        spotX = getRandomInt(0, m_map->w - 1);
        spotY = getRandomInt(0, m_map->h - 1);

        makeSpot(SPoint(spotX, spotY), Terrain_Rock);
    }

    // Spice fields

    for (i = 0; i < m_spiceFields; i++)
    {
        spotX = getRandomInt(0, m_map->w - 1);
        spotY = getRandomInt(0, m_map->h - 1);

        makeSpot(SPoint(spotX, spotY), Terrain_Spice);
    }

    for (count = 0; count < ROCKFILLER; count++)
        thickSpots(Terrain_Rock); //SPOT ROCK

    for (count = 0; count < SPICEFILLER; count++)
        thickSpots(Terrain_Spice);

    addRockBits();

    addBlooms();

    smoothTerrain();

    return true;
}



void MapGenerator::takeMapScreenshot(std::string filename)
{
    int w = m_map->w;
    int h = m_map->h;
    Image * img = new Image(UPoint(w * 16, h * 16));


    for (int i = 0 ; i < w; i++)
    {
        for (int j = 0 ; j < h; j++)
        {
            m_map->getCell(UPoint(i, j))->draw(img, SPoint(16*i, 16*j));
        }
    }

    SDL_SaveBMP(img->getSurface(), filename.c_str());

    delete img;
}


void MapGenerator::addPlayer(PLAYERHOUSE House, bool ai, int team)
{
    if (m_gs->m_players->size() > (unsigned)House)
    {
        LOG_ERROR("MapGenerator" , "Trying to create already existing player!");
        exit(EXIT_FAILURE);
    }

    if (ai == true)
    {
        //player[House] = new AiPlayerClass(House,House,House,DEFAULT_STARTINGCREDITS,InitSettings->Difficulty,team);
        LOG_WARNING("MapGenerator" , "Trying to create unimplemented ai player!");
    }

    else
    {
        m_gs->m_players->push_back(new PlayerClass(House, House, House, DEFAULT_STARTINGCREDITS, team, m_gs));
    }

    m_gs->m_players->at(House)->assignMapPlayerNum(House);
}

void MapGenerator::addRockBits()
{
    int spotX, spotY;
    int done = 0;
    TerrainClass * tmp;

    while (done < m_rockBits)
    {
        spotX = getRandomInt(0, m_map->w - 1);
        spotY = getRandomInt(0, m_map->h - 1);
        tmp = m_map->getCell(SPoint(spotX, spotY));

        if (tmp->getType() == Terrain_Sand)
        {
            tmp->setTile(Terrain_t10);      // Rock bit
            tmp->setType(Terrain_Rock);
        }

        done++;
    }
}

void MapGenerator::addBlooms()
{
    int spotX, spotY;
    int done = 0;
    TerrainClass * tmp;

    while (done < m_spiceBlooms)
    {
        spotX = getRandomInt(0, m_map->w - 1);
        spotY = getRandomInt(0, m_map->h - 1);
        tmp = m_map->getCell(SPoint(spotX, spotY));

        if (tmp->getType() == Terrain_Sand && tmp->getTile() == Terrain_a1)
            tmp->setTile(getRandomInt(Terrain_a2, Terrain_a3));     // Spice bloom

        done++;
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

void MapGenerator::makeSpot(SPoint cellPos, int type)
{
    int dir;  // Direction
    int j;   // Loop control

    for (j = 0; j < 1000; j++)
    {
        dir = getRandomInt(0, 3); // Random Dir

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
                    if (getRandomInt(0, 1) == 1)
                        m_map->getCell(SPoint(i, j))->setType(type);
                }
            }
        }
}

vector<string> MapGenerator::SplitString(string ParseString)
{
    vector<string> retVector;
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
