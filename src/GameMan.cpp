#include <eastwood/IniFile.h>
#include <stack>

#include "GameMan.h"
#include "ResMan.h"
#include "PythonObjects.h"

#include "objects/BuilderClass.h"
#include "objects/StructureClass.h"
#include "objects/AirUnit.h"
#include "objects/GroundUnit.h"
#include "objects/InfantryClass.h"

uint8_t lookDist[11];

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

void GameMan::AddPlayer(HOUSETYPE House, bool ai, int team)
{
    if (m_players.size() > (unsigned)House)
    {
	LOG(LV_ERROR, "GameMan" , "Trying to create already existing player!");
	exit(EXIT_FAILURE);
    }

    if (ai == true)
    {
	//player[House] = new AiPlayerClass(House,House,House,DEFAULT_STARTINGCREDITS,InitSettings->Difficulty,team);
	LOG(LV_WARNING, "GameMan" , "Trying to create unimplemented ai player!");
    }

    else
    {
	m_players.push_back(new PlayerClass(House, House, House, DEFAULT_STARTINGCREDITS, team));
	m_localPlayer = m_players[0];//localPlayer;
    }

    m_players[House]->assignMapPlayerNum(House);
}

void GameMan::Init()
{
    m_objectIDCounter = 0;
}

void GameMan::Clear()
{
    for(std::vector<PlayerClass*>::const_iterator iter = m_players.begin(); iter != m_players.end(); iter++)
	delete *iter;
    m_players.clear();
    m_objects.clear();

    delete m_map;
}

ObjectPtr GameMan::getObject(uint32_t objectID) {
    ObjectPtr object;
    for(ObjectTypeMap::const_iterator iter = getObjectsBegin(); iter != getObjectsEnd(); iter++)
    {
	ObjectMap::const_iterator objMap = (*iter).second.find(objectID);
	if(objMap != (*iter).second.end())
	    return objMap->second;
    }

    return object;
}

uint32_t GameMan::addObject(ObjectPtr object) {
    uint32_t objectID = ++m_objectIDCounter,
	     objectType = object->getAttributes();

    if(m_objects.find(objectType) == m_objects.end())
	m_objects[objectType] = ObjectMap();
    m_objects[objectType][objectID] = object;
    object->setObjectID(objectID);


    return objectID;
}

void GameMan::removeObject(uint32_t objectID) {
    for(ObjectTypeMap::const_iterator iter = getObjectsBegin(); iter != getObjectsEnd(); iter++)
	if(((ObjectMap)iter->second).erase(objectID))
	    return;
}

ObjectPtr GameMan::createObject(std::string itemName, PlayerClass* Owner)
{
    std::string objectClass;
    ObjectPtr newObject;
    try {
	objectClass = getPyObjectType(DataCache::Instance()->getPyObject(itemName));
    }
    catch(python::error_already_set const &)
    {
	PyErr_Print();
    }

    if(objectClass == "AirUnit")
	newObject.reset(new AirUnit(Owner, itemName));
    else if(objectClass == "GroundUnit")
	newObject.reset(new GroundUnit(Owner, itemName));
    else if(objectClass == "InfantryUnit")
	newObject.reset(new InfantryClass(Owner, itemName));
    else if(objectClass == "Object")
	newObject.reset(new ObjectClass(Owner, itemName));
    /*    else if(objectClass == "Unit")
	  newObject = new UnitClass(Owner, itemName);
	  */
    else if(objectClass == "Builder")
	newObject.reset(new BuilderClass(Owner, itemName));
    else if(objectClass == "Structure")
	newObject.reset(new StructureClass(Owner, itemName));
    else
	LOG(LV_ERROR, "GameMan", "createObject: item %s, %s unknown type!", itemName.c_str(), objectClass.c_str());

    addObject(newObject);

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
	LOG(LV_ERROR, "GameMan", "Cannot find Seednum in %s!", scenarioName.c_str());
	delete m_map;
	return false;
    }

    string FieldString = myInifile->getStringValue("MAP", "Field");
    string BloomString = myInifile->getStringValue("MAP", "Bloom");

    m_map = MapGenerator::Instance()->createOldMap(FieldString, SeedNum, BloomString);
    // now set up all the players

    AddPlayer(HOUSE_ATREIDES, false, 1);
    AddPlayer(HOUSE_ORDOS, false, 2);
    AddPlayer(HOUSE_HARKONNEN, false, 2);
    AddPlayer(HOUSE_SARDAUKAR, false, 2);
    AddPlayer(HOUSE_FREMEN, false, 2);
    AddPlayer(HOUSE_MERCENARY, false, 2);

    IniFile::KeyListHandle myListHandle;

    myListHandle = myInifile->KeyList_Open("UNITS");

    while (!myInifile->KeyList_EOF(myListHandle))
    {
	string tmpkey = myInifile->KeyList_GetNextKey(&myListHandle);
	string tmp = myInifile->getStringValue("UNITS", tmpkey);
	string HouseStr, UnitStr;
	int health, pos;
	sScanf(tmp, "%S,%S,%d,%d", &HouseStr, &UnitStr, &health, &pos);

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
	    LOG(LV_WARNING, "GameMan", "LoadScenario: Invalid house string: %s", HouseStr.c_str());
	    house = HOUSE_ATREIDES;
	}

	if (pos <= 0)
	{
	    LOG(LV_WARNING, "GameMan", "LoadScenario: Invalid position string: %d", pos);
	    pos = 0;
	}

	ObjectPtr newUnit = m_players[house]->placeUnit(UnitStr, UPoint(pos % 64, pos / 64));
	if (!newUnit)
	    LOG(LV_WARNING, "GameMan", "LoadScenario: This file is not a valid unit entry: %d. (invalid unit position)", pos);
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
	    int pos = atoi(PosStr.c_str());

	    string HouseStr, BuildingStr;
	    sScanf(tmp, "%S,%S", &HouseStr, &BuildingStr);

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
		LOG(LV_WARNING, "GameMan", "LoadScenario: Invalid house string: %s", HouseStr.c_str());
		house = HOUSE_ATREIDES;
	    }

	    /*
	    //FIXME: Fix this here and in addPlayer
	    if(m_players->size() > house) {
	    LOG(LV_ERROR, "MapGenerator","player[%d]== NULL",(int) house);
	    exit(EXIT_FAILURE);
	    }
	    */


	    //Using INVALID_POS instead of NONE to avoid warnings.
	    //FIXME: Maybe we should rename INVALID_POS to INVALID or sth
	    if (BuildingStr == "Concrete")
		m_players[house]->placeStructure(INVALID_POS, INVALID_POS, "Slab1", UPoint(pos % 64, pos / 64));
	    else if (BuildingStr == "Wall")
		m_players[house]->placeStructure(INVALID_POS, INVALID_POS, "Wall", UPoint(pos % 64, pos / 64));
	    else
		LOG(LV_WARNING, "GameMan", "LoadScenario: Invalid building string: %s", BuildingStr.c_str());
	}

	else
	{
	    // other structure
	    string HouseStr, BuildingStr;
	    int health, pos;
	    sScanf(tmp, "%S,%S,%d,%d", &HouseStr, &BuildingStr, &health, &pos);

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
		LOG(LV_WARNING, "GameMan", "LoadScenario: Invalid house string: %s", HouseStr.c_str());
		house = HOUSE_ATREIDES;
	    }

	    //Using INVALID_POS instead of NONE to avoid warnings.
	    //FIXME: Maybe we should rename INVALID_POS to INVALID or sth
	    ObjectPtr newStructure = m_players[house]->placeStructure(INVALID_POS, INVALID_POS, BuildingStr, UPoint(pos % 64, pos / 64));

	    if (newStructure == NULL)
		LOG(LV_WARNING, "GameMan", "LoadScenario: Invalid position: %d", pos);
	}
    }

    myInifile->KeyList_Close(&myListHandle);
    return true;
}

/*
   void GameMan::Select(List* objectList)
   {

   }
   */

void GameMan::TakeMapScreenshot(string filename)
{
    int w = m_map->w;
    int h = m_map->h;
    Image * img = new Image(UPoint(w * 16, h * 16));


    for (int i = 0 ; i < w; i++)
	for (int j = 0 ; j < h; j++)
	    m_map->getCell(UPoint(i, j))->draw(img, SPoint(16*i, 16*j));

    img->saveBMP(filename);

    delete img;
}

/*
   void GameMan::Unselect(List* objectList)
   {

   }
   */
void GameMan::Update(float dt)
{
    for(ObjectTypeMap::const_iterator objTypeMap = m_objects.begin(); objTypeMap != m_objects.end(); objTypeMap++)
    {
	ObjectMap ObjMap = (*objTypeMap).second;
	std::stack<uint32_t> eraseStack;
	for(ObjectMap::const_iterator objMap = ObjMap.begin(); objMap != ObjMap.end(); objMap++)
	{
	    ObjectPtr object = objMap->second;
	    if (object->clearObject())
		eraseStack.push(objMap->first);
	    else
		object->update(dt);
	}
	while(!eraseStack.empty())
	{
	    ObjMap.erase(eraseStack.top());
	    eraseStack.pop();
	}
    }
}
