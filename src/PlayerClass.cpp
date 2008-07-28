#include "DataCache.h"
#include "GameState.h"
#include "houses.h"
#include "Log.h"
#include "PlayerClass.h"
#include "MapClass.h"

//FIXME:When it's done these includes should be moved to Items.h
#include "structures/BarracksClass.h"
#include "structures/ConstructionYardClass.h"
#include "structures/HeavyFactoryClass.h"
#include "structures/HighTechFactoryClass.h"
#include "structures/GunTurretClass.h"
#include "structures/IXClass.h"
#include "structures/LightFactoryClass.h"
#include "structures/PalaceClass.h"
#include "structures/RefineryClass.h"
#include "structures/SiloClass.h"
#include "structures/StructureClass.h"
#include "structures/WallClass.h"
#include "structures/WORClass.h"
#include "units/UnitClass.h"
#include "units/QuadClass.h"

PlayerClass::PlayerClass(int newPlayerNumber, int newHouse, int newColour, int newCredits, int team, GameState* gs)
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
    
    m_gs = gs;

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

void* PlayerClass::createUnit(int itemID)

{

	UnitClass* newUnit = NULL;
	switch (itemID)
	{
/*
	case (Unit_Carryall):
		newUnit = new Carryall(this);
		numCarryalls++;
		break;

	case (Unit_Devastator):
		newUnit = new DevastatorClass(this);
		break;
	case (Unit_Deviator):
		newUnit = new DeviatorClass(this);
		break;
	case (Unit_Frigate):
		newUnit = new Frigate(this);
		break;
	case (Unit_Harvester):
		newUnit = new HarvesterClass(this);
		numHarvesters++;
		break;
	case (Unit_Infantry):
		newUnit = new InfantryClass(this);
		break;
	case (Unit_Launcher):
		newUnit = new LauncherClass(this);
		break;
	case (Unit_MCV):
		newUnit = new MCVClass(this);
		break;
	case (Unit_Ornithopter):
		newUnit = new Ornithopter(this);
		break;
*/
	case (Unit_Quad):
		newUnit = new QuadClass(this);
		break;
/*
	case (Unit_Saboteur):
		newUnit = new Saboteur(this);
		break;
	case (Unit_Sandworm):
		newUnit = new Sandworm(this);
		break;
	case (Unit_SiegeTank):
		newUnit = new SiegeTankClass(this);
		break;
	case (Unit_SonicTank):
		newUnit = new SonicTankClass(this);
		break;
	case (Unit_Tank):
		newUnit = new TankClass(this);
		break;
	case (Unit_Trike):
		newUnit = new TrikeClass(this);
		break;
	case (Unit_Raider):
		newUnit = new RaiderClass(this);
		break;
	case (Unit_Trooper):
		newUnit = new TrooperClass(this);
		break;
	case (Unit_Sardaukar):
		newUnit = new SardaukarClass(this);
		break;
	case (Unit_Fremen):
		newUnit = new FremenClass(this);
		break;
	
*/  
    default:
        break;
    }

	if (newUnit)
	{
        newUnit->setObjectID(m_gs->getObjectID());
		m_gs->m_units->push_back(newUnit);
		if (itemID != Unit_Sandworm)
			m_numUnits++;
	}

	return newUnit;
}

MapClass* PlayerClass::getMap()
{
    return m_gs->m_map;
}

void* PlayerClass::placeUnit(int itemID, UPoint itemPos)
{

	UnitClass* newUnit = NULL;
	if (m_gs->m_map->cellExists(itemPos))
		newUnit = (UnitClass*)createUnit(itemID);

	if (newUnit)
	{
		if (newUnit->canPass(itemPos))
			newUnit->deploy(itemPos);
		else
		{
//			newUnit->setVisible(VIS_ALL, false);
//			newUnit->destroy();
			newUnit = NULL;
		}
	}
	return newUnit;
}

void* PlayerClass::placeStructure(int builderID, UPoint builderPos, int itemID, UPoint itemPos)
{
    m_numStructures++;

    StructureClass* tempStructure = NULL;

    switch (itemID)
    {
        case (Structure_Barracks):
		tempStructure = new BarracksClass(this);
            break;

        case (Structure_ConstructionYard):
		tempStructure = new ConstructionYardClass(this);
            break;

        case (Structure_GunTurret):
		tempStructure = new GunTurretClass(this);
            break;

        case (Structure_HeavyFactory):
		tempStructure = new HeavyFactoryClass(this);
            break;

        case (Structure_HighTechFactory):
		tempStructure = new HighTechFactoryClass(this);
            break;

        case (Structure_IX):
		tempStructure = new IXClass(this);
            break;

        case (Structure_LightFactory):
		tempStructure = new LightFactoryClass(this);
            break;

        case (Structure_Palace):
		tempStructure = new PalaceClass(this);
            break;

        case (Structure_Refinery):
		tempStructure = new RefineryClass(this);
    	        addCapacity(2000);
            break;

        case (Structure_Silo):
		tempStructure = new SiloClass(this);
    	        addCapacity(100);
	    break;
		
	case (Structure_Wall):
                tempStructure = new WallClass(this);
                 m_numWalls++;
	    break;
	    
        case (Structure_WOR):
		tempStructure = new WORClass(this);
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

				if (m_gs->m_map->cellExists(UPoint(itemPos.x+i, itemPos.y+j)))
				{
					//m_map->getCell(UPoint(itemPos.x+i, itemPos.y+j))->clearDamage();

				}
			}
		}



        tempStructure->setPosition(itemPos);

        if (itemID == Structure_Wall)
            m_gs->m_map->fixWalls(itemPos.x, itemPos.y);

        tempStructure->setObjectID(m_gs->getObjectID());
        m_gs->m_structures->push_back(tempStructure);
    }
       
    return tempStructure;
}
