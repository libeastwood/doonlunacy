#include "DataCache.h"
#include "houses.h"
#include "Log.h"
#include "PlayerClass.h"

//FIXME:When it's done these includes should be moved to Items.h
#include "structures/SiloClass.h"
#include "structures/StructureClass.h"
#include "structures/WallClass.h"


PlayerClass::PlayerClass(int newPlayerNumber, int newHouse, int newColour, int newCredits, int team, MapClass* map, List * list)
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
	m_team = newPlayerNumber;
	
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
    
    m_map = map;
    m_structureList = list;

    Log::Instance()->setVerbosity("PlayerClass", LV_MAX);
}

PlayerClass::~PlayerClass()
{

}

void PlayerClass::assignMapPlayerNum(int newMapPlayerNum)
{
	if ((newMapPlayerNum >=0) && (newMapPlayerNum < MAX_PLAYERS))
		m_mapPlayerNum = newMapPlayerNum;
	else
		m_mapPlayerNum = 0;
}

void* PlayerClass::placeStructure(int builderID, UPoint builderPos, int itemID, UPoint itemPos)
{
    m_numStructures++;
    
    StructureClass* tempStructure = NULL;
    
    switch (itemID)
    {
        case (Structure_Silo):
		    tempStructure = new SiloClass(this);
    	    addCapacity(100);
	    	break;
		
		case (Structure_Wall):
    		tempStructure = new WallClass(this);
            m_numWalls++;
	        break;
	    
	    default:
	        m_numStructures--;
    	    break;
    }
    
    if (tempStructure)
	{
		for(int i=0;i<tempStructure->w;i++)
		{
			for(int j=0;j<tempStructure->h;j++)
			{
			    //FIXME: No idea what it does
				LOG_INFO("PlayerClass", "Clear x: %d, y: %d", itemPos.x + i, itemPos.y + j);

				if (m_map->cellExists(UPoint(itemPos.x+i, itemPos.y+j)))
				{
					LOG_INFO("PlayerClass", "Clearing damage");
					//m_map->getCell(UPoint(itemPos.x+i, itemPos.y+j))->clearDamage();

				}
			}
		}



		tempStructure->setPosition(itemPos);

//		if (itemID == Structure_Wall)
//			m_map->fixWalls(itemPos.x, itemPos.y);
        tempStructure->setLocation(itemPos);
		m_structureList->push_back(tempStructure);
    }
       
    return tempStructure;
}
