#include "DataCache.h" 


using namespace std;

DataCache::DataCache() {
    for (int i=0; i< NUM_HOUSES; i++)
    {
        m_objImg.push_back(new images());
    }

    int len;
    unsigned char * data;
    
   	Icnfile* icon;
   	Shpfile* units;
	Shpfile* units1;
	Shpfile* units2;
	
    //LOADING FILES    
    data = ResMan::Instance()->readFile("DUNE:UNITS.SHP", &len);
    units = new Shpfile(data, len);
    
    data = ResMan::Instance()->readFile("DUNE:UNITS1.SHP", &len);
    units1 = new Shpfile(data, len);
    
    data = ResMan::Instance()->readFile("DUNE:UNITS2.SHP", &len);
    units2 = new Shpfile(data, len);
    
    
    int maplen;
    unsigned char * mapdata;
    
    data = ResMan::Instance()->readFile("DUNE:ICON.ICN", &len);
    mapdata = ResMan::Instance()->readFile("DUNE:ICON.MAP", &maplen);
    icon = new Icnfile(data, len, mapdata, maplen);


    //UNITS, BUILDINGS, EXPLOSIONS, and everything that's on the map
	addObjImg(ObjImg_Tank_Base, units2->getPictureArray(8,1,GROUNDUNIT_ROW(0)));
	addObjImg(ObjImg_Tank_Gun, units2->getPictureArray(8,1,GROUNDUNIT_ROW(5)));	
	addObjImg(ObjImg_Siegetank_Base, units2->getPictureArray(8,1,GROUNDUNIT_ROW(10)));
	addObjImg(ObjImg_Siegetank_Gun, units2->getPictureArray(8,1,GROUNDUNIT_ROW(15)));
	addObjImg(ObjImg_Devastator_Base, units2->getPictureArray(8,1,GROUNDUNIT_ROW(20)));
	addObjImg(ObjImg_Devastator_Gun, units2->getPictureArray(8,1,GROUNDUNIT_ROW(25)));	
	addObjImg(ObjImg_Sonictank_Gun, units2->getPictureArray(8,1,GROUNDUNIT_ROW(30)));
	addObjImg(ObjImg_Launcher_Gun, units2->getPictureArray(8,1,GROUNDUNIT_ROW(35)));	
	addObjImg(ObjImg_Quad, units->getPictureArray(8,1,GROUNDUNIT_ROW(0)));	
	addObjImg(ObjImg_Trike, units->getPictureArray(8,1,GROUNDUNIT_ROW(5)));		
	addObjImg(ObjImg_Harvester, units->getPictureArray(8,1,GROUNDUNIT_ROW(10)));
	addObjImg(ObjImg_Harvester_Sand, units1->getPictureArray(8,3,HARVESTERSAND_ROW(72),HARVESTERSAND_ROW(73),HARVESTERSAND_ROW(74)));
	addObjImg(ObjImg_MCV, units->getPictureArray(8,1,GROUNDUNIT_ROW(15)));
	addObjImg(ObjImg_Carryall, units->getPictureArray(8,2,AIRUNIT_ROW(45),AIRUNIT_ROW(48)));
	addObjImg(ObjImg_Frigate, units->getPictureArray(8,1,AIRUNIT_ROW(60)));
	addObjImg(ObjImg_Ornithopter, units->getPictureArray(8,3,ORNITHOPTER_ROW(51),ORNITHOPTER_ROW(52),ORNITHOPTER_ROW(53)));
	addObjImg(ObjImg_Trooper, units->getPictureArray(4,3,INFANTRY_ROW(82),INFANTRY_ROW(83),INFANTRY_ROW(84)));
	addObjImg(ObjImg_Infantry, units->getPictureArray(4,3,INFANTRY_ROW(73),INFANTRY_ROW(74),INFANTRY_ROW(75)));
	addObjImg(ObjImg_Saboteur, units->getPictureArray(4,3,INFANTRY_ROW(63),INFANTRY_ROW(64),INFANTRY_ROW(65)));
	addObjImg(ObjImg_Sandworm, units1->getPictureArray(1,5,67|TILE_NORMAL,68|TILE_NORMAL,69|TILE_NORMAL,70|TILE_NORMAL,71|TILE_NORMAL));
	addObjImg(ObjImg_ConstructionYard, icon->getPictureArray(17));
	addObjImg(ObjImg_Windtrap, icon->getPictureArray(19));
	addObjImg(ObjImg_Refinery, icon->getPictureArray(21));
	addObjImg(ObjImg_Barracks, icon->getPictureArray(18));
	addObjImg(ObjImg_WOR, icon->getPictureArray(16));
	addObjImg(ObjImg_Radar, icon->getPictureArray(26));
	addObjImg(ObjImg_LightFactory, icon->getPictureArray(12));
	addObjImg(ObjImg_Silo, icon->getPictureArray(25));
	addObjImg(ObjImg_HeavyFactory, icon->getPictureArray(13));
	addObjImg(ObjImg_HighTechFactory, icon->getPictureArray(14));
	addObjImg(ObjImg_IX, icon->getPictureArray(15));
	addObjImg(ObjImg_Palace, icon->getPictureArray(11));
	addObjImg(ObjImg_RepairYard, icon->getPictureArray(22));
	addObjImg(ObjImg_Starport, icon->getPictureArray(20));
	addObjImg(ObjImg_GunTurret, icon->getPictureArray(23));
	addObjImg(ObjImg_RocketTurret, icon->getPictureArray(24));
	addObjImg(ObjImg_Wall, icon->getPictureArray(6,1,1,75));
	addObjImg(ObjImg_Bullet_SmallRocket, units->getPictureArray(16,1,ROCKET_ROW(35)));
	addObjImg(ObjImg_Bullet_MediumRocket, units->getPictureArray(16,1,ROCKET_ROW(20)));
	addObjImg(ObjImg_Bullet_LargeRocket, units->getPictureArray(16,1,ROCKET_ROW(40)));
	addObjImg(ObjImg_Bullet_Small, units1->getPicture(23));
	addObjImg(ObjImg_Bullet_Medium, units1->getPicture(24));
	addObjImg(ObjImg_Bullet_Sonic, units1->getPicture(9));
	addObjImg(ObjImg_Hit_Gas, units1->getPictureArray(5,1,57|TILE_NORMAL,58|TILE_NORMAL,59|TILE_NORMAL,60|TILE_NORMAL,61|TILE_NORMAL));
	addObjImg(ObjImg_Hit_Shell, units1->getPictureArray(3,1,2|TILE_NORMAL,3|TILE_NORMAL,4|TILE_NORMAL));
	addObjImg(ObjImg_ExplosionSmall, units1->getPictureArray(5,1,32|TILE_NORMAL,33|TILE_NORMAL,34|TILE_NORMAL,35|TILE_NORMAL,36|TILE_NORMAL));
	addObjImg(ObjImg_ExplosionMedium1, units1->getPictureArray(5,1,47|TILE_NORMAL,48|TILE_NORMAL,49|TILE_NORMAL,50|TILE_NORMAL,51|TILE_NORMAL));
	addObjImg(ObjImg_ExplosionMedium2, units1->getPictureArray(5,1,52|TILE_NORMAL,53|TILE_NORMAL,54|TILE_NORMAL,55|TILE_NORMAL,56|TILE_NORMAL));
	addObjImg(ObjImg_ExplosionLarge1, units1->getPictureArray(5,1,37|TILE_NORMAL,38|TILE_NORMAL,39|TILE_NORMAL,40|TILE_NORMAL,41|TILE_NORMAL));
	addObjImg(ObjImg_ExplosionLarge2, units1->getPictureArray(5,1,42|TILE_NORMAL,43|TILE_NORMAL,44|TILE_NORMAL,45|TILE_NORMAL,46|TILE_NORMAL));
	addObjImg(ObjImg_ExplosionSmallUnit, units1->getPictureArray(2,1,0|TILE_NORMAL,1|TILE_NORMAL));
	addObjImg(ObjImg_DeadInfantry, icon->getPictureArray(4,1,1,6));
	addObjImg(ObjImg_Smoke, units1->getPictureArray(3,1,29|TILE_NORMAL,30|TILE_NORMAL,31|TILE_NORMAL));
	addObjImg(ObjImg_SandwormShimmerMask, units1->getPicture(10));
	addObjImg(ObjImg_Terrain, icon->getPictureRow(124,209));
	addObjImg(ObjImg_RockDamage, icon->getPictureRow(1,6));
	addObjImg(ObjImg_SandDamage, units1->getPictureArray(3,1,5|TILE_NORMAL,6|TILE_NORMAL,7|TILE_NORMAL));
	addObjImg(ObjImg_Terrain_Hidden, icon->getPictureRow(108,123));
}

void DataCache::addObjImg(unsigned ID, SDL_Surface * tmp) {

    m_objImg[HOUSE_HARKONNEN]->insert(pair<unsigned, ImagePtr>(ID, 
                                      ImagePtr(new Image(tmp))));
}

ImagePtr DataCache::getObjImg(unsigned ID, unsigned house) {

    images::iterator iter = m_objImg[house]->find(ID);
    if (iter != m_objImg[house]->end())
    { 
        return m_objImg[house]->find(ID)->second;
    }
    else
    {
        ImagePtr source = m_objImg[HOUSE_HARKONNEN]->find(ID)->second;
        ImagePtr copy = source->getRecoloredByHouse(house);
        m_objImg[HOUSE_HARKONNEN]->insert(pair<unsigned, ImagePtr>(ID, copy));
        return copy;
    }

}

DataCache::~DataCache() {

}
