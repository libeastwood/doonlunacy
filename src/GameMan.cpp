#include "GameMan.h"

#include "ResMan.h"

#include "structures/ConstructionYardClass.h"
#include "units/AirUnit.h"
#include "units/GroundUnit.h"
#include "units/InfantryClass.h"

#include <eastwood/IniFile.h>

using namespace std;


int lookDist[11];

GameMan::GameMan()
{
    lookDist[0] = 10; 
    lookDist[1] = 10;
    lookDist[2] = 9;
    lookDist[3] = 9;
    lookDist[4] = 9;
    lookDist[5] = 8;
    lookDist[6] = 8;
    lookDist[7] = 7;
    lookDist[8] = 6;
    lookDist[9] = 4;
    lookDist[10] = 1;

    Init();
}

GameMan::~GameMan()
{
    Clear();
}

void GameMan::AddPlayer(PLAYERHOUSE House, bool ai, int team)
{
    if (m_players->size() > (unsigned)House)
    {
        LOG_ERROR("GameMan" , "Trying to create already existing player!");
        exit(EXIT_FAILURE);
    }

    if (ai == true)
    {
        //player[House] = new AiPlayerClass(House,House,House,DEFAULT_STARTINGCREDITS,InitSettings->Difficulty,team);
        LOG_WARNING("GameMan" , "Trying to create unimplemented ai player!");
    }

    else
    {
        PlayerClass * localPlayer = new PlayerClass(House, House, House, DEFAULT_STARTINGCREDITS, team);
        m_players->push_back(localPlayer);
        m_localPlayer = m_players->at(0);//localPlayer;
    }

    m_players->at(House)->assignMapPlayerNum(House);
}

void GameMan::Init()
{
    m_players = new Players;
    m_structures = new Structures;
    m_units = new Units;
	m_objectTree = new ObjectTree();
    m_bulletList = new Bullets;

    m_bulletList->push_back(new BulletClass(NULL, UPoint(50,50), UPoint(498, 352), (int)Bullet_LargeRocket, false)); 
    m_bulletList->push_back(new BulletClass(NULL, UPoint(50,200), UPoint(498, 352), (int)Bullet_LargeRocket, false)); 
    m_bulletList->push_back(new BulletClass(NULL, UPoint(200,50), UPoint(498, 352), (int)Bullet_LargeRocket, false)); 
    m_bulletList->push_back(new BulletClass(NULL, UPoint(400,400), UPoint(498, 352), (int)Bullet_LargeRocket, false)); 
    m_bulletList->push_back(new BulletClass(NULL, UPoint(200,400), UPoint(498, 352), (int)Bullet_LargeRocket, false)); 

}

void GameMan::Clear()
{
    for (unsigned int i = 0; i < m_players->size(); i++)
    {
        delete m_players->at(i);
    }
    m_players->clear();
    
    for (unsigned int i = 0; i < m_units->size(); i++)
    {
        UnitClass* unit = m_units->back(); 
        m_objectTree->RemoveObject(unit->getObjectID());
        m_units->pop_back();
        delete unit;
    }
    m_units->clear();
    
    delete m_map;
}

/*static*/
ObjectClass* GameMan::CreateObject(int ItemID, PlayerClass* Owner, Uint32 ObjectID)
{
	ObjectClass* newObject = NULL;
    //FIXME: Temporarily create quads only
	switch (ItemID)
	{
	#if 0
		case Structure_Barracks:			newObject = new BarracksClass(Owner); break;
		case Structure_ConstructionYard:	newObject = new ConstructionYardClass(Owner); break;
		case Structure_GunTurret:			newObject = new GunTurretClass(Owner); break;
		case Structure_HeavyFactory:		newObject = new HeavyFactoryClass(Owner); break;
		case Structure_HighTechFactory:		newObject = new HighTechFactoryClass(Owner); break;
		case Structure_IX:					newObject = new IXClass(Owner); break;
		case Structure_LightFactory:		newObject = new LightFactoryClass(Owner); break;
		case Structure_Palace:				newObject = new PalaceClass(Owner); break;
		case Structure_Radar:				newObject = new RadarClass(Owner); break;
		case Structure_Refinery:			newObject = new RefineryClass(Owner); break;
		case Structure_RepairYard:			newObject = new RepairYardClass(Owner); break;
		case Structure_RocketTurret:		newObject = new RocketTurretClass(Owner); break;
		case Structure_Silo:				newObject = new SiloClass(Owner); break;
		case Structure_StarPort:			newObject = new StarPortClass(Owner); break;
		case Structure_Wall:				newObject = new WallClass(Owner); break;
		case Structure_WindTrap:			newObject = new WindTrapClass(Owner); break;
		case Structure_WOR:					newObject = new WORClass(Owner); break;
        #endif
		
		case Unit_Carryall:				newObject = new AirUnit(Owner, "Carryall"); break;
		case Unit_Devastator:				newObject = new GroundUnit(Owner, "Devastator"); break;
		case Unit_Deviator:				newObject = new GroundUnit(Owner, "Deviator"); break;
		case Unit_Fremen:				newObject = new InfantryClass(Owner, "Fremen"); break;
		case Unit_Frigate:				newObject = new AirUnit(Owner, "Frigate"); break;
		case Unit_Harvester:				newObject = new GroundUnit(Owner, "Harvester"); break;
		case Unit_Infantry:				newObject = new InfantryClass(Owner, "Infantry"); break;
		case Unit_Launcher:				newObject = new GroundUnit(Owner, "Launcher"); break;
		case Unit_MCV:					newObject = new GroundUnit(Owner, "MCV"); break;
		case Unit_Ornithopter:				newObject = new AirUnit(Owner, "Ornithopter"); break;
		case Unit_Quad:					newObject = new GroundUnit(Owner, "Quad"); break;
		case Unit_Saboteur:				newObject = new GroundUnit(Owner, "Saboteur"); break;
		case Unit_Sandworm:				newObject = new GroundUnit(Owner, "Sandworm"); break;
		case Unit_SiegeTank:				newObject = new GroundUnit(Owner, "Siege Tank"); break;
		case Unit_SonicTank:				newObject = new GroundUnit(Owner, "Sonic Tank"); break;
		case Unit_Tank:					newObject = new GroundUnit(Owner, "Tank"); break;
		case Unit_Trike:				newObject = new GroundUnit(Owner, "Trike"); break;
		case Unit_Raider:				newObject = new GroundUnit(Owner, "Raider"); break;
		case Unit_Trooper:				newObject = new InfantryClass(Owner, "Trooper"); break;
		case Unit_Sardaukar:				newObject = new InfantryClass(Owner, "Sardaukar"); break;
		case Structure_ConstructionYard:		newObject = new ConstructionYardClass(Owner); break;
		default:					newObject = NULL;
								LOG_ERROR("GameMan", "%d is no valid ItemID!",ItemID);
								break;
	}
	
	if(newObject == NULL)
		return NULL;
	
	if(ObjectID == NONE) {
		ObjectID = GetObjectTree()->AddObject(newObject);
		newObject->setObjectID(ObjectID);
	} else {
		newObject->setObjectID(ObjectID);
	}
	
	return newObject;
}

bool GameMan::LoadScenario(string scenarioName)
{
	size_t len;
	uint8_t *data = ResMan::Instance()->readFile(scenarioName, &len);

    IniFile *myInifile = new IniFile(data, len);

    int SeedNum = myInifile->getIntValue("MAP", "Seed", -1);

    if (SeedNum == -1)
    {
        LOG_ERROR("GameMan", "Cannot find Seednum in %s!", scenarioName.c_str());
        delete m_map;
        return false;
    }

    string FieldString = myInifile->getStringValue("MAP", "Field");
    string BloomString = myInifile->getStringValue("MAP", "Bloom");
    
    m_map = MapGenerator::Instance()->createOldMap(FieldString, SeedNum, BloomString);
    // now set up all the players

    AddPlayer(ATREIDES, false, 1);
    AddPlayer(ORDOS, false, 2);
    AddPlayer(HARKONNEN, false, 2);
    AddPlayer(SARDAUKAR, false, 2);
    AddPlayer(FREMEN, false, 2);
    AddPlayer(MERCENERY, false, 2);

    IniFile::KeyListHandle myListHandle;

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
            LOG_WARNING("GameMan", "LoadScenario: Invalid house string: %s", HouseStr.c_str());
            house = HOUSE_ATREIDES;
        }

        int pos = atol(PosStr.c_str());

        if (pos <= 0)
        {
            LOG_WARNING("GameMan", "LoadScenario: Invalid position string: %s", PosStr.c_str());
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
            LOG_WARNING("GameMan", "LoadScenario: Invalid unit string: %s", UnitStr.c_str());
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
                LOG_WARNING("GameMan", "LoadScenario: This file is not a valid unit entry: %d. (invalid unit position)", pos);
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
                LOG_WARNING("GameMan", "LoadScenario: Invalid house string: %s", HouseStr.c_str());
                house = HOUSE_ATREIDES;
            }

            /*
                //FIXME: Fix this here and in addPlayer
                if(m_players->size() > house) {
                LOG_ERROR("MapGenerator","player[%d]== NULL",(int) house);
                exit(EXIT_FAILURE);
               }
            */


            //Using INVALID_POS instead of NONE to avoid warnings.
            //FIXME: Maybe we should rename INVALID_POS to INVALID or sth
            if (BuildingStr == "Concrete")
            {
                m_players->at(house)->placeStructure(INVALID_POS, INVALID_POS, Structure_Slab1, UPoint(pos % 64, pos / 64));
            }

            else if (BuildingStr == "Wall")
            {
                m_players->at(house)->placeStructure(INVALID_POS, INVALID_POS, Structure_Wall, UPoint(pos % 64, pos / 64));
            }

            else
            {
                LOG_WARNING("GameMan", "LoadScenario: Invalid building string: %s", BuildingStr.c_str());
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
                LOG_WARNING("GameMan", "LoadScenario: Invalid house string: %s", HouseStr.c_str());
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
                LOG_WARNING("GameMan", "LoadScenario: Invalid building: %s", BuildingStr.c_str());
                itemID = 0;
            }

            if ((m_players->at(house) != NULL) && (itemID != 0))
            {
                //Using INVALID_POS instead of NONE to avoid warnings.
                //FIXME: Maybe we should rename INVALID_POS to INVALID or sth
                ObjectClass*  newStructure = (ObjectClass*)m_players->at(house)->placeStructure(INVALID_POS, INVALID_POS, itemID, UPoint(pos % 64, pos / 64));

                if (newStructure == NULL)
                {
                    LOG_WARNING("GameMan", "LoadScenario: Invalid position: %s", PosStr.c_str());
                }
            }

        }
    }

    myInifile->KeyList_Close(&myListHandle);
    return true;
}

void GameMan::Select(List* objectList)
{

}

/*
 Splits a string into several substrings. This strings are separated with ','.
*/
bool GameMan::SplitString(string ParseString, unsigned int NumStringPointers, ...)
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

void GameMan::TakeMapScreenshot(string filename)
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

void GameMan::Unselect(List* objectList)
{

}

void GameMan::Update(float dt)
{
    Units::iterator unit = m_units->begin();
    UnitClass* tmp;
    while (unit != m_units->end())
    {
        tmp = *unit;
        if (tmp->clearObject())
        {
            m_objectTree->RemoveObject(tmp->getObjectID());
            unit++;
        }
        else
        {
            unit++;
        }
    }
    
    for (unit = m_units->begin(); unit != m_units->end(); unit++)
    {
        (*unit)->update(dt);
    }
    
    Bullets::iterator bullet;
    for (bullet = m_bulletList->begin(); bullet != m_bulletList->end(); bullet++)
    {
        (*bullet)->updatePosition(dt);
    }
}
