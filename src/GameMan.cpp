#include <eastwood/IniFile.h>

#include "GameMan.h"
#include "ResMan.h"

#include "objects/BuilderClass.h"
#include "objects/StructureClass.h"
#include "objects/AirUnit.h"
#include "objects/GroundUnit.h"
#include "objects/InfantryClass.h"

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
    m_objectTree = new ObjectTree();
    m_objects = new std::vector<ObjectClass*>;

}

void GameMan::Clear()
{
    for (unsigned int i = 0; i < m_players->size(); i++)
    {
        delete m_players->at(i);
    }
    m_players->clear();
    
    /*
    for (unsigned int i = 0; i < m_units->size(); i++)
    {
        UnitClass* unit = m_units->back(); 
        m_objectTree->RemoveObject(unit->getObjectID());
        m_units->pop_back();
        delete unit;
    }
    m_units->clear();
    */
    
    delete m_map;
}

ObjectClass* GameMan::createObject(std::string itemName, PlayerClass* Owner, Uint32 ObjectID)
{
    std::string objectClass;
    ObjectClass* newObject = NULL;
    try {
	objectClass = DataCache::Instance()->getPyObjectType(itemName);
    }
    catch(python::error_already_set const &)
    {
	PyErr_Print();
    }

    if(objectClass == "AirUnit")
	    newObject = new AirUnit(Owner, itemName);
    else if(objectClass == "GroundUnit")
	    newObject = new GroundUnit(Owner, itemName);
    else if(objectClass == "InfantryUnit")
	    newObject = new InfantryClass(Owner, itemName);
    else if(objectClass == "Object")
	    newObject = new ObjectClass(Owner, itemName);
/*    else if(objectClass == "Unit")
	    newObject = new UnitClass(Owner, itemName);
	    */
    else if(objectClass == "Builder")
	    newObject = new BuilderClass(Owner, itemName);
    else if(objectClass == "Structure")
	    newObject = new StructureClass(Owner, itemName);
    else
	LOG_ERROR("GameMan", "createObject: item %s, %s unknown type!", itemName.c_str(), objectClass.c_str());

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

        int pos = atoi(PosStr.c_str());

        if (pos <= 0)
        {
            LOG_WARNING("GameMan", "LoadScenario: Invalid position string: %s", PosStr.c_str());
            pos = 0;
        }

        int Num2Place = 1;

            //make three

        if (UnitStr == "Infantry")
            Num2Place = 3;
        else if (UnitStr == "Troopers")
            Num2Place = 3;

        /*        //FIXME: Fix this here and in addPlayer
          if(m_players->size() > house) {
           LOG_ERROR("MapGenerator", "player[%d]== NULL",(int) house);
           exit(EXIT_FAILURE);
          }
        */

        for (int i = 0; i < Num2Place; i++)
        {
            ObjectClass* newUnit = (ObjectClass*)m_players->at(house)->placeUnit(UnitStr, UPoint(pos % 64, pos / 64));

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
                m_players->at(house)->placeStructure(INVALID_POS, INVALID_POS, "Slab1", UPoint(pos % 64, pos / 64));
            }

            else if (BuildingStr == "Wall")
            {
                m_players->at(house)->placeStructure(INVALID_POS, INVALID_POS, "Wall", UPoint(pos % 64, pos / 64));
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

            int pos = atoi(PosStr.c_str());

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

	   
            if (m_players->at(house) != NULL)
            {
                //Using INVALID_POS instead of NONE to avoid warnings.
                //FIXME: Maybe we should rename INVALID_POS to INVALID or sth
                ObjectClass*  newStructure = (ObjectClass*)m_players->at(house)->placeStructure(INVALID_POS, INVALID_POS, BuildingStr, UPoint(pos % 64, pos / 64));

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
    std::vector<ObjectClass*>::iterator object;
    for (object = m_objects->begin(); object != m_objects->end(); object++)
    {
	if ((*object)->clearObject())
	    m_objectTree->RemoveObject((*object)->getObjectID());
	else
	    (*object)->update(dt);
    }
}
