#include "DataCache.h"
#include "Definitions.h"
#include "GameMan.h"
#include "houses.h"
#include "Log.h"
#include "MapClass.h"
#include "PlayerClass.h"
#include "SoundPlayer.h"

//FIXME:When it's done these includes should be moved to Items.h
#include "ObjectClass.h"

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
    LOG_INFO("PlayerClass", "Player deleted");
}

void PlayerClass::assignMapPlayerNum(int newMapPlayerNum)
{
	if ((newMapPlayerNum >=0) && (newMapPlayerNum < MAX_PLAYERS))
		m_mapPlayerNum = newMapPlayerNum;
	else
		m_mapPlayerNum = 0;
}

UnitClass* PlayerClass::createUnit(int itemID)
{
	UnitClass* newUnit = NULL;

	newUnit = (UnitClass*) GameMan::Instance()->CreateObject(itemID,this);

	if(newUnit == NULL) 
	{
		LOG_ERROR("PlayerClass", "Cannot create Object with itemID %d",itemID);
		exit(EXIT_FAILURE);
	}

	if (itemID != Unit_Sandworm) 
	{
		m_numUnits++;
	}

	switch (itemID)	{
		case (Unit_Frigate): 
			m_numFrigates++;
			break;

		case (Unit_Carryall): 
			m_numCarryalls++;
			break;

		case (Unit_Harvester):
			m_numHarvesters++;
			break;
	}

	return newUnit;
}


void PlayerClass::decrementUnits(int itemID)
{
	if (itemID != Unit_Sandworm)
		m_numUnits--;
		
	switch (itemID)
	{
	case (Unit_Carryall):
		m_numCarryalls--;
		break;
	case (Unit_Frigate):
		m_numFrigates--;
		break;
	case (Unit_Harvester):
		//decrementHarvesters();
		LOG_INFO("PlayerClass", "Decrement harvesters not implemented");
		break;
	default:
		break;
	}

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

void* PlayerClass::placeStructure(int builderID, UPoint builderPos, int itemID, UPoint itemPos)
{
	GameMan* gman = GameMan::Instance();
	MapClass* map = gman->GetMap();

	if(!map->cellExists(itemPos)) {
		return NULL;
	}
	
	StructureClass* tempStructure = NULL;
	
	if((itemID != Structure_Slab1) && (itemID != Structure_Slab4)) {
		tempStructure = (StructureClass*) GameMan::Instance()->CreateObject(itemID,this);
		if(tempStructure == NULL) {
			LOG_ERROR("PlayerClass", "Cannot create Object with itemID %d",itemID);
			exit(EXIT_FAILURE);
		}
		
		//numStructures++;
	}
	
	switch (itemID)
	{	
		case (Structure_Slab1):
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
		} break;
	
		case (Structure_Slab4):
		{
			// Slabs are no normal buildings
			int i,j;
			for(i = itemPos.x; i < itemPos.x + 2; i++) {
				for(j = itemPos.y; j < itemPos.y + 2; j++) {
					if (map->cellExists(i, j)) {
						TerrainClass* cell = map->getCell(i, j);
			
						if (!cell->hasAGroundObject() && cell->isRock() && !cell->isMountain()) {
							cell->setType(Structure_Slab1);
							cell->setOwner(getPlayerNumber());
							map->viewMap(getTeam(), i, j, TILE_VIEWRANGE);
							//cell->clearDamage();
						}
					}
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
		} break;	
	}

	if (tempStructure) {
		for(int i=0;i<tempStructure->w;i++) {
			for(int j=0;j<tempStructure->h;j++) {
				//sprintf(temp, "clear x: %d, y: %d", xPos + i, yPos + j);
				//currentGame->AddToNewsTicker(temp);

				if(map->cellExists(itemPos.x+i, itemPos.y+j)) {
					map->getCell(itemPos.x+i, itemPos.y+j)->clearDamage();
				}
			}
		}

		tempStructure->setPosition(itemPos);

		if (itemID == Structure_Wall)
			map->fixWalls(itemPos.x, itemPos.y);

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

	return tempStructure;
}

UnitClass* PlayerClass::placeUnit(int itemID, UPoint itemPos)
{
	UnitClass* newUnit = NULL;
	if (GameMan::Instance()->GetMap()->cellExists(itemPos))
		newUnit = (UnitClass*)createUnit(itemID);

	if (newUnit)
	{
		if (newUnit->canPass(itemPos))
			newUnit->deploy(itemPos);
		else
		{
			newUnit->setVisible(VIS_ALL, false);
			newUnit->destroy();
			newUnit = NULL;
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
