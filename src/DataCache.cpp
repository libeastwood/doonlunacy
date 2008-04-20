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
	addObjPic(ObjPic_Tank_Base, units2->getPictureArray(8,1,GROUNDUNIT_ROW(0)));
	addObjPic(ObjPic_Tank_Gun, units2->getPictureArray(8,1,GROUNDUNIT_ROW(5)));	
	addObjPic(ObjPic_Siegetank_Base, units2->getPictureArray(8,1,GROUNDUNIT_ROW(10)));
	addObjPic(ObjPic_Siegetank_Gun, units2->getPictureArray(8,1,GROUNDUNIT_ROW(15)));
	addObjPic(ObjPic_Devastator_Base, units2->getPictureArray(8,1,GROUNDUNIT_ROW(20)));
	addObjPic(ObjPic_Devastator_Gun, units2->getPictureArray(8,1,GROUNDUNIT_ROW(25)));	
	addObjPic(ObjPic_Sonictank_Gun, units2->getPictureArray(8,1,GROUNDUNIT_ROW(30)));
	addObjPic(ObjPic_Launcher_Gun, units2->getPictureArray(8,1,GROUNDUNIT_ROW(35)));	
	addObjPic(ObjPic_Quad, units->getPictureArray(8,1,GROUNDUNIT_ROW(0)));	
	addObjPic(ObjPic_Trike, units->getPictureArray(8,1,GROUNDUNIT_ROW(5)));		
	addObjPic(ObjPic_Harvester, units->getPictureArray(8,1,GROUNDUNIT_ROW(10)));
	addObjPic(ObjPic_Harvester_Sand, units1->getPictureArray(8,3,HARVESTERSAND_ROW(72),HARVESTERSAND_ROW(73),HARVESTERSAND_ROW(74)));
	addObjPic(ObjPic_MCV, units->getPictureArray(8,1,GROUNDUNIT_ROW(15)));
	addObjPic(ObjPic_Carryall, units->getPictureArray(8,2,AIRUNIT_ROW(45),AIRUNIT_ROW(48)));
	addObjPic(ObjPic_Frigate, units->getPictureArray(8,1,AIRUNIT_ROW(60)));
	addObjPic(ObjPic_Ornithopter, units->getPictureArray(8,3,ORNITHOPTER_ROW(51),ORNITHOPTER_ROW(52),ORNITHOPTER_ROW(53)));
	addObjPic(ObjPic_Trooper, units->getPictureArray(4,3,INFANTRY_ROW(82),INFANTRY_ROW(83),INFANTRY_ROW(84)));
	addObjPic(ObjPic_Infantry, units->getPictureArray(4,3,INFANTRY_ROW(73),INFANTRY_ROW(74),INFANTRY_ROW(75)));
	addObjPic(ObjPic_Saboteur, units->getPictureArray(4,3,INFANTRY_ROW(63),INFANTRY_ROW(64),INFANTRY_ROW(65)));
	addObjPic(ObjPic_Sandworm, units1->getPictureArray(1,5,67|TILE_NORMAL,68|TILE_NORMAL,69|TILE_NORMAL,70|TILE_NORMAL,71|TILE_NORMAL));
	addObjPic(ObjPic_ConstructionYard, icon->getPictureArray(17));
	addObjPic(ObjPic_Windtrap, icon->getPictureArray(19));
	addObjPic(ObjPic_Refinery, icon->getPictureArray(21));
	addObjPic(ObjPic_Barracks, icon->getPictureArray(18));
	addObjPic(ObjPic_WOR, icon->getPictureArray(16));
	addObjPic(ObjPic_Radar, icon->getPictureArray(26));
	addObjPic(ObjPic_LightFactory, icon->getPictureArray(12));
	addObjPic(ObjPic_Silo, icon->getPictureArray(25));
	addObjPic(ObjPic_HeavyFactory, icon->getPictureArray(13));
	addObjPic(ObjPic_HighTechFactory, icon->getPictureArray(14));
	addObjPic(ObjPic_IX, icon->getPictureArray(15));
	addObjPic(ObjPic_Palace, icon->getPictureArray(11));
	addObjPic(ObjPic_RepairYard, icon->getPictureArray(22));
	addObjPic(ObjPic_Starport, icon->getPictureArray(20));
	addObjPic(ObjPic_GunTurret, icon->getPictureArray(23));
	addObjPic(ObjPic_RocketTurret, icon->getPictureArray(24));
	addObjPic(ObjPic_Wall, icon->getPictureArray(6,1,1,75));
	addObjPic(ObjPic_Bullet_SmallRocket, units->getPictureArray(16,1,ROCKET_ROW(35)));
	addObjPic(ObjPic_Bullet_MediumRocket, units->getPictureArray(16,1,ROCKET_ROW(20)));
	addObjPic(ObjPic_Bullet_LargeRocket, units->getPictureArray(16,1,ROCKET_ROW(40)));
	addObjPic(ObjPic_Bullet_Small, units1->getPicture(23));
	addObjPic(ObjPic_Bullet_Medium, units1->getPicture(24));
	addObjPic(ObjPic_Bullet_Sonic, units1->getPicture(9));
	addObjPic(ObjPic_Hit_Gas, units1->getPictureArray(5,1,57|TILE_NORMAL,58|TILE_NORMAL,59|TILE_NORMAL,60|TILE_NORMAL,61|TILE_NORMAL));
	addObjPic(ObjPic_Hit_Shell, units1->getPictureArray(3,1,2|TILE_NORMAL,3|TILE_NORMAL,4|TILE_NORMAL));
	addObjPic(ObjPic_ExplosionSmall, units1->getPictureArray(5,1,32|TILE_NORMAL,33|TILE_NORMAL,34|TILE_NORMAL,35|TILE_NORMAL,36|TILE_NORMAL));
	addObjPic(ObjPic_ExplosionMedium1, units1->getPictureArray(5,1,47|TILE_NORMAL,48|TILE_NORMAL,49|TILE_NORMAL,50|TILE_NORMAL,51|TILE_NORMAL));
	addObjPic(ObjPic_ExplosionMedium2, units1->getPictureArray(5,1,52|TILE_NORMAL,53|TILE_NORMAL,54|TILE_NORMAL,55|TILE_NORMAL,56|TILE_NORMAL));
	addObjPic(ObjPic_ExplosionLarge1, units1->getPictureArray(5,1,37|TILE_NORMAL,38|TILE_NORMAL,39|TILE_NORMAL,40|TILE_NORMAL,41|TILE_NORMAL));
	addObjPic(ObjPic_ExplosionLarge2, units1->getPictureArray(5,1,42|TILE_NORMAL,43|TILE_NORMAL,44|TILE_NORMAL,45|TILE_NORMAL,46|TILE_NORMAL));
	addObjPic(ObjPic_ExplosionSmallUnit, units1->getPictureArray(2,1,0|TILE_NORMAL,1|TILE_NORMAL));
	addObjPic(ObjPic_DeadInfantry, icon->getPictureArray(4,1,1,6));
	addObjPic(ObjPic_Smoke, units1->getPictureArray(3,1,29|TILE_NORMAL,30|TILE_NORMAL,31|TILE_NORMAL));
	addObjPic(ObjPic_SandwormShimmerMask, units1->getPicture(10));
	addObjPic(ObjPic_Terrain, icon->getPictureRow(124,209));
	addObjPic(ObjPic_RockDamage, icon->getPictureRow(1,6));
	addObjPic(ObjPic_SandDamage, units1->getPictureArray(3,1,5|TILE_NORMAL,6|TILE_NORMAL,7|TILE_NORMAL));
	addObjPic(ObjPic_Terrain_Hidden, icon->getPictureRow(108,123));
}

void DataCache::addObjPic(unsigned ID, SDL_Surface * tmp) {

    m_objImg[HOUSE_HARKONNEN]->insert(pair<unsigned, ImagePtr>(ID, 
                                      ImagePtr(new Image(tmp))));
}

ImagePtr DataCache::getObjPic(unsigned ID, unsigned house) {

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

/*void DataCache::addVoice(unsigned ID, std::string){
	
}*/

DataCache::~DataCache() {

}
