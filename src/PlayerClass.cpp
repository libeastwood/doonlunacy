#include "DataCache.h"
#include "Definitions.h"
#include "GameMan.h"
#include "houses.h"
#include "Log.h"
#include "MapClass.h"
#include "PlayerClass.h"
#include "SoundPlayer.h"

//FIXME:When it's done these includes should be moved to Items.h
#include "objects/ObjectClass.h"

PlayerClass::PlayerClass(int newPlayerNumber, int newHouse, int newColour, int newCredits, int team)
{
    m_mapPlayerNum = newPlayerNumber;

    if ((newHouse >= 0) && (newHouse < NUM_HOUSES))
	m_house = newHouse;
    else
	m_house = 0;

    if ((newColour >= 0) && (newColour < NUM_HOUSES))
	m_colour = newColour;
    else
	m_colour = 0;

    m_capacity = 0;
    m_power = 0;
    m_powerRequirement = 0;
    m_team = team;

    m_numBarracks = 0;
    m_numCarryalls = 0;
    m_numFrigates = 0;
    m_numHarvesters = 0;
    m_numUnits = 0;
    m_numIXs = 0;
    m_numHeavyFactories = 0;
    m_numHighTechFactories = 0;
    m_numLightFactories = 0;
    m_numPalaces = 0;
    m_numRefineries = 0;
    m_numRepairYards = 0;
    m_numRocketTurrets = 0;
    m_numRadars = 0;
    m_numStarPorts = 0;
    m_numWalls = 0;
    m_numWindTraps = 0;
    m_numWORs = 0;
    m_numWalls = 0;
    m_numStructures = 0;
}

PlayerClass::~PlayerClass()
{
    LOG_INFO("PlayerClass", "Player %d (house: %d) deleted", m_mapPlayerNum, m_house);
}

void PlayerClass::assignMapPlayerNum(int newMapPlayerNum)
{
    if ((newMapPlayerNum >=0) && (newMapPlayerNum < MAX_PLAYERS))
	m_mapPlayerNum = newMapPlayerNum;
    else
	m_mapPlayerNum = 0;
}

ObjectPtr PlayerClass::createUnit(std::string itemName)
{
    ObjectPtr newUnit = GameMan::Instance()->createObject(itemName,this);

    if (itemName != "Sandworm") 
    {
	m_numUnits++;
    }


    if(itemName == "Frigate")
	m_numFrigates++;
    else if(itemName == "Carryall")
	m_numCarryalls++;
    else if(itemName == "Harvester")
	m_numHarvesters++;

    return newUnit;
}

void PlayerClass::decrementUnits(std::string itemName)
{
    if (itemName != "Sandworm")
	m_numUnits--;

    if(itemName == "Carryall")
	m_numCarryalls--;
    else if(itemName == "Frigate")
	m_numFrigates--;
    else if(itemName == "Harvester")
	m_numHarvesters--;
    //decrementHarvesters();
    //		LOG_INFO("PlayerClass", "Decrement harvesters not implemented");

    //if (!isAlive())
    //	lose();

    LOG_INFO("PlayerClass", "numUnits:%d", m_numUnits);

    //currentGame->AddToNewsTicker(temp);
}


/*inline*/
MapClass* PlayerClass::getMap()
{
    return GameMan::Instance()->GetMap();
}

ObjectPtr PlayerClass::placeStructure(int builderID, UPoint builderPos, std::string itemName, UPoint itemPos)
{
    ObjectPtr structure;
    GameMan* gman = GameMan::Instance();
    MapClass* map = gman->GetMap();

    if(!map->cellExists(itemPos)) {
	return structure;
    }


    if((itemName != "Slab1") && (itemName != "Slab4")) {
	structure = GameMan::Instance()->createObject(itemName,this);
	if(!structure) {
	    LOG_ERROR("PlayerClass", "Cannot create Object %s", itemName.c_str());
	    exit(EXIT_FAILURE);
	}

	//numStructures++;
    }

    if(itemName == "Slab1")
    {
	// Slabs are no normal buildings
	map->getCell(itemPos)->setType(Structure_Slab1);
	map->getCell(itemPos)->setOwner(getPlayerNumber());
	map->viewMap(getTeam(), itemPos, TILE_VIEWRANGE);
	//		map->getCell(xPos, yPos)->clearDamage();

	/*
	   if (builderID != NONE) {
	   ObjectClass*	object = map->findObjectWithID(builderID, builderPos);
	   if (object->isAStructure() && ((StructureClass*)object)->isABuilder()) {
	   ((BuilderClass*)object)->unSetWaitingToPlace();
	   if (this == thisPlayer) {
	   currentGame->placingMode = false;
	   }
	   }
	   } else if (this == thisPlayer) {
	   currentGame->placingMode = false;
	   }
	   */			
    }
    else if(itemName == "Slab4")
    {
	// Slabs are no normal buildings
	int i,j;
	for(i = itemPos.x; i < itemPos.x + 2; i++)
	    for(j = itemPos.y; j < itemPos.y + 2; j++)
		if (map->cellExists(i, j)) {
		    SPoint pos(i, j);
		    TerrainClass* cell = map->getCell(pos);

		    if (!cell->hasAGroundObject() && cell->isRock() && !cell->isMountain()) {
			cell->setType(Structure_Slab1);
			cell->setOwner(getPlayerNumber());
			map->viewMap(getTeam(), i, j, TILE_VIEWRANGE);
			//cell->clearDamage();
		    }
		}
	/*	
		if (builderID != NONE) {
		ObjectClass*	object = map->findObjectWithID(builderID, builderPos);

		if (object->isAStructure() && ((StructureClass*)object)->isABuilder()) {
		((BuilderClass*)object)->unSetWaitingToPlace();

		if (this == thisPlayer) {
		currentGame->placingMode = false;
		}
		}
		} else if (this == thisPlayer) {
		currentGame->placingMode = false;
		}
		*/			
    }

    if (structure) {
	for(int i=0;i<structure->w;i++)
	    for(int j=0;j<structure->h;j++) {
		//sprintf(temp, "clear x: %d, y: %d", xPos + i, yPos + j);
		//currentGame->AddToNewsTicker(temp);

		SPoint pos(itemPos.x+i, itemPos.y+j);
		if(map->cellExists(pos))
		    map->getCell(pos)->clearDamage();
	    }

	structure->setPosition(itemPos);

	if (itemName == "Wall")
	    map->fixWalls(itemPos);

	// at the beginning of the game every refinery gets one harvester for free (brought by a carryall)
	//		if ((gameState == BEGUN) && (itemID == Structure_Refinery))
	//			freeHarvester(itemPos);
	/*
	// if this structure was built by a construction yard this construction yard must be informed
	if (builderID != NONE) {
	ObjectClass*	object = map->findObjectWithID(builderID, builderPos);
	if (object->isAStructure() && ((StructureClass*)object)->isABuilder()) {	
	((BuilderClass*)object)->unSetWaitingToPlace();
	if (this == thisPlayer) {
	currentGame->placingMode = false;
	}
	}
	} else if (this == thisPlayer) {
	currentGame->placingMode = false;
	}

	sapPower(tempStructure->getPowerRequirement());

	if(itemID == Structure_Radar && hasPower()) {
	changeRadar(true);
	}

	if(!loading)
	checkSelectionLists();
	*/			
    }

    return structure;
}

ObjectPtr PlayerClass::placeUnit(std::string itemName, UPoint itemPos)
{
    ObjectPtr newUnit;
    if (GameMan::Instance()->GetMap()->cellExists(itemPos))
	newUnit = createUnit(itemName);

    if (newUnit)
    {
	UnitClass *unit = (UnitClass*)newUnit.get();
	if (unit->canPass(itemPos))
	    unit->deploy(itemPos);
	else
	{
	    newUnit->setVisible(VIS_ALL, false);
	    newUnit->destroy();
	    newUnit.reset();
	}
    }

    return newUnit;
}

void PlayerClass::update()
{
    if (m_oldCredits != m_credits)
    {
	//		if ((this == GameMan::Instance()->LocalPlayer()) && (getAmountOfCredits() > 0))
	//			SoundPlayer::Instance()->playSound(CreditsTick);
	m_oldCredits = m_credits;
    }

    if (m_credits > m_capacity)
    {
	m_credits--;// = capacity;
	if (this == GameMan::Instance()->LocalPlayer())
	{
	    //TODO: Add news ticker, etc.:)
	    //currentGame->AddToNewsTicker("spice lost, build more silos.");
	}
    }
}
