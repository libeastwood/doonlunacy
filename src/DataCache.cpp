#include "DataCache.h" 
#include "pakfile/Vocfile.h"

DataCache::DataCache() {
    for (int i=0; i< NUM_HOUSES; i++)
    {
        m_objImg.push_back(new images());
		m_guiImg.push_back(new images());
    }

    int len;
    unsigned char * data;
    
   	Icnfile* icon;
   	Shpfile* units;
	Shpfile* units1;
	Shpfile* units2;
	Cpsfile* herald;
	Shpfile* mentat;
//	CpsfilePtr mentatm;
	

	
    //LOADING FILES    
    data = ResMan::Instance()->readFile("DUNE:UNITS.SHP", &len);
    units = new Shpfile(data, len);
    
    data = ResMan::Instance()->readFile("DUNE:UNITS1.SHP", &len);
    units1 = new Shpfile(data, len);
    
    data = ResMan::Instance()->readFile("DUNE:UNITS2.SHP", &len);
    units2 = new Shpfile(data, len);

	data = ResMan::Instance()->readFile("ENGLISH:HERALD.ENG", &len);
    herald = new Cpsfile(data, len);

	data = ResMan::Instance()->readFile("ENGLISH:MENTAT.ENG", &len);
	mentat = new Shpfile(data, len);

	data = ResMan::Instance()->readFile("DUNE:BENE.PAL", &len);
    
    data = ResMan::Instance()->readFile("DUNE:MENTATM.CPS", &len);

	CpsfilePtr mentatm (new Cpsfile(data, len));

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

	//addGuiPic(UI_HouseChoiceBackground, herald->getPicture());
		
	addGuiPic(UI_MentatYes, mentat->getPicture(0));
	addGuiPic(UI_MentatYes_Pressed, mentat->getPicture(1));
	addGuiPic(UI_MentatNo, mentat->getPicture(2));
	addGuiPic(UI_MentatNo_Pressed, mentat->getPicture(3));

	data = ResMan::Instance()->readFile("DUNE:BENE.PAL", &len);
	Palettefile tmp (data, len);
	SDL_Palette * pal = tmp.getPalette();
	addGuiPic(UI_Mentat_BeneGesserit, mentatm->getPicture(pal));

	addSoundChunk(YesSir, getChunkFromFile("VOC:ZREPORT1.VOC"));
	addSoundChunk(Reporting, getChunkFromFile("VOC:ZREPORT2.VOC"));
	addSoundChunk(Acknowledged, getChunkFromFile("VOC:ZREPORT3.VOC"));
	addSoundChunk(Affirmative, getChunkFromFile("VOC:ZAFFIRM.VOC"));
	addSoundChunk(MovingOut, getChunkFromFile("VOC:ZMOVEOUT.VOC"));
	addSoundChunk(InfantryOut, getChunkFromFile("VOC:ZOVEROUT.VOC"));
	addSoundChunk(SomethingUnderTheSand, getChunkFromFile("VOC:SANDBUG.VOC"));
	addSoundChunk(HouseAtreides, getChunkFromFile("GERMAN:GATRE.VOC"));
	addSoundChunk(HouseOrdos, getChunkFromFile("GERMAN:GORDOS.VOC"));
	addSoundChunk(HouseHarkonnen, getChunkFromFile("GERMAN:GHARK.VOC"));

	addSoundChunk(PlaceStructure, getChunkFromFile("VOC:EXDUD.VOC"));
	addSoundChunk(ButtonClick, getChunkFromFile("VOC:BUTTON.VOC"));
//	addSoundChunk(InvalidAction, Mix_LoadWAV_RW(pFileManager->OpenFile("CANNOT.WAV"),1));
	// probably incorrect?
	addSoundChunk(CreditsTick, getChunkFromFile("INTROVOC:CLICK.VOC"));
	addSoundChunk(RadarNoise, getChunkFromFile("VOC:STATICP.VOC"));
	addSoundChunk(Sound_ExplosionGas, getChunkFromFile("VOC:EXGAS.VOC"));
	addSoundChunk(Sound_ExplosionTiny, getChunkFromFile("VOC:EXTINY.VOC"));
	addSoundChunk(Sound_ExplosionSmall, getChunkFromFile("VOC:EXSMALL.VOC"));
	addSoundChunk(Sound_ExplosionMedium, getChunkFromFile("VOC:EXMED.VOC"));
	addSoundChunk(Sound_ExplosionLarge, getChunkFromFile("VOC:EXLARGE.VOC"));
	addSoundChunk(Sound_ExplosionStructure, getChunkFromFile("VOC:CRUMBLE.VOC"));
	addSoundChunk(Sound_WormAttack, getChunkFromFile("VOC:WORMET3P.VOC"));
	addSoundChunk(Sound_Gun, getChunkFromFile("VOC:GUN.VOC"));
	addSoundChunk(Sound_Rocket, getChunkFromFile("VOC:ROCKET.VOC"));
	addSoundChunk(Sound_Bloom, getChunkFromFile("VOC:EXSAND.VOC"));
	addSoundChunk(Sound_Scream1, getChunkFromFile("VOC:VSCREAM1.VOC"));
	addSoundChunk(Sound_Scream2, getChunkFromFile("VOC:VSCREAM1.VOC"));
	addSoundChunk(Sound_Scream3, getChunkFromFile("VOC:VSCREAM1.VOC"));
	addSoundChunk(Sound_Scream4, getChunkFromFile("VOC:VSCREAM1.VOC"));
	addSoundChunk(Sound_Scream5, getChunkFromFile("VOC:VSCREAM1.VOC"));
	addSoundChunk(Sound_MachineGun, getChunkFromFile("VOC:GUNMULTI.VOC"));
//	addSoundChunk(Sound_Sonic, Mix_LoadWAV_RW(pFileManager->OpenFile("SONIC.WAV"),1);
	addSoundChunk(Sound_RocketSmall, getChunkFromFile("VOC:MISLTINP.VOC"));

	addSoundChunk(Intro_3Houses, getChunkFromFile("INTROVOC:3HOUSES.VOC"));
	addSoundChunk(Intro_AndNow, getChunkFromFile("INTROVOC:ANDNOW.VOC"));
	addSoundChunk(Intro_Battle, getChunkFromFile("INTROVOC:ARRIVED.VOC"));
	addSoundChunk(Intro_Begins, getChunkFromFile("INTROVOC:BEGINS.VOC"));
	addSoundChunk(Intro_Blaster, getChunkFromFile("INTROVOC:BLASTER.VOC"));
	addSoundChunk(Intro_TheBuilding, getChunkFromFile("INTROVOC:BLDING.VOC"));
	addSoundChunk(Intro_Blowup_1, getChunkFromFile("INTROVOC:BLOWUP1.VOC"));
	addSoundChunk(Intro_Blowup_2, getChunkFromFile("INTROVOC:BLOWUP2.VOC"));
	addSoundChunk(Intro_Brakes_2p, getChunkFromFile("INTROVOC:BRAKES2P.VOC"));
	addSoundChunk(Intro_Clank, getChunkFromFile("INTROVOC:CLANK.VOC"));
	addSoundChunk(Intro_Click, getChunkFromFile("INTROVOC:CLICK.VOC"));
	addSoundChunk(Intro_ControlsDune, getChunkFromFile("INTROVOC:CONTROL2.VOC"));
	addSoundChunk(Intro_ControlsTheSpice, getChunkFromFile("INTROVOC:CONTROL3.VOC"));
	addSoundChunk(Intro_ForControl, getChunkFromFile("INTROVOC:CONTROL4.VOC"));
	addSoundChunk(Intro_Controls, getChunkFromFile("INTROVOC:CONTROLS.VOC"));
	addSoundChunk(Intro_Dune, getChunkFromFile("INTROVOC:DUNE.VOC"));
	addSoundChunk(Intro_OfADynasty, getChunkFromFile("INTROVOC:DYNASTY.VOC"));
	addSoundChunk(Intro_ToEachOfTheHouses, getChunkFromFile("INTROVOC:EACHHOME.VOC"));
	addSoundChunk(Intro_AndNo, getChunkFromFile("INTROVOC:EANDNO.VOC"));
	addSoundChunk(Intro_WillControlDune, getChunkFromFile("INTROVOC:ECONTROL.VOC"));
	addSoundChunk(Intro_TheHouse, getChunkFromFile("INTROVOC:EHOUSE.VOC"));
	addSoundChunk(Intro_TheMostSpice, getChunkFromFile("INTROVOC:EMOST.VOC"));
	addSoundChunk(Intro_TheEmpire, getChunkFromFile("INTROVOC:EMPIRE.VOC"));
	addSoundChunk(Intro_ThereAreNoSetTerritories, getChunkFromFile("INTROVOC:ENOSET.VOC"));
	addSoundChunk(Intro_ThatProduces, getChunkFromFile("INTROVOC:EPRODUCE.VOC"));
	addSoundChunk(Intro_RulesOfEngagement, getChunkFromFile("INTROVOC:ERULES.VOC"));
	addSoundChunk(Intro_Territories, getChunkFromFile("INTROVOC:ETERRIT.VOC"));
	addSoundChunk(Intro_AndThe, getChunkFromFile("INTROVOC:EVIL.VOC"));
//	addSoundChunk(Intro_Filler, getChunkFromFile("INTROVOC:FILLER.VOC"));
	addSoundChunk(Intro_Flesh, getChunkFromFile("INTROVOC:FLESH.VOC"));
	addSoundChunk(Intro_Glass_6, getChunkFromFile("INTROVOC:GLASS6.VOC"));
	addSoundChunk(Intro_Glass, getChunkFromFile("INTROVOC:GLASS.VOC"));
	addSoundChunk(Intro_GunShot, getChunkFromFile("INTROVOC:GUNSHOT.VOC"));
	addSoundChunk(Intro_EvilHarkonnen, getChunkFromFile("INTROVOC:HARK.VOC"));
	addSoundChunk(Intro_Home, getChunkFromFile("INTROVOC:HOME.VOC"));
	addSoundChunk(Intro_OnlyOneHouse, getChunkFromFile("INTROVOC:HOUSE2.VOC"));
	addSoundChunk(Intro_TheInsideous, getChunkFromFile("INTROVOC:INSID.VOC"));
	addSoundChunk(Intro_TheEmperor, getChunkFromFile("INTROVOC:KING.VOC"));
	addSoundChunk(Intro_KnownAsDune, getChunkFromFile("INTROVOC:KNOWN.VOC"));
	addSoundChunk(Intro_Worm, getChunkFromFile("INTROVOC:LIZARD1.VOC"));
	addSoundChunk(Intro_Melange, getChunkFromFile("INTROVOC:MELANGE.VOC"));
	addSoundChunk(Intro_Missile_8, getChunkFromFile("INTROVOC:MISSLE8.VOC"));
	addSoundChunk(Intro_TheNobleAtreides, getChunkFromFile("INTROVOC:NOBLE.VOC"));
	addSoundChunk(Intro_Now, getChunkFromFile("INTROVOC:NOW.VOC"));
	addSoundChunk(Intro_OfDune, getChunkFromFile("INTROVOC:OFDUNE.VOC"));
	addSoundChunk(Intro_Ordos, getChunkFromFile("INTROVOC:ORD.VOC"));
	addSoundChunk(Intro_ThePlanetArrakis, getChunkFromFile("INTROVOC:PLANET.VOC"));
	addSoundChunk(Intro_WillPrevail, getChunkFromFile("INTROVOC:PREVAIL.VOC"));
	addSoundChunk(Intro_HasProposedAChallenge, getChunkFromFile("INTROVOC:PROPOSED.VOC"));
	addSoundChunk(Intro_LandOfSand, getChunkFromFile("INTROVOC:SANDLAND.VOC"));
	addSoundChunk(Intro_OfTheSpice, getChunkFromFile("INTROVOC:SPICE.VOC"));
	addSoundChunk(Intro_TheSpice, getChunkFromFile("INTROVOC:SPICE2.VOC"));
	addSoundChunk(Intro_VastArmies, getChunkFromFile("INTROVOC:VAST.VOC"));
	addSoundChunk(Intro_WhoEver, getChunkFromFile("INTROVOC:WHOEVER.VOC"));
	addSoundChunk(Intro_Wind_2bp, getChunkFromFile("INTROVOC:WIND2BP.VOC"));
	addSoundChunk(Intro_Your, getChunkFromFile("INTROVOC:YOUR.VOC"));
}

void DataCache::addObjPic(ObjPic_enum ID, Image * tmp, HOUSETYPE house) {

    m_objImg[house]->insert(std::pair<ObjPic_enum, ImagePtr>(ID, 
                                      ImagePtr(tmp)));
}

void DataCache::addGuiPic(GuiPic_enum ID, Image * tmp, HOUSETYPE house) {

    m_guiImg[house]->insert(std::pair<GuiPic_enum, ImagePtr>(ID, 
                                      ImagePtr(tmp)));
}

ImagePtr DataCache::getObjPic(ObjPic_enum ID, HOUSETYPE house) {

    images::iterator iter = m_objImg[house]->find(ID);
    if (iter != m_objImg[house]->end())
    { 
        return m_objImg[house]->find(ID)->second;
    }
    else
    {
        ImagePtr source = m_objImg[house]->find(ID)->second;
        ImagePtr copy = source->getRecoloredByHouse(house);
        m_objImg[house]->insert(std::pair<ObjPic_enum, ImagePtr>(ID, copy));
        return copy;
    }

}

ImagePtr DataCache::getGuiPic(GuiPic_enum ID, HOUSETYPE house) {

    images::iterator iter = m_guiImg[house]->find(ID);
    if (iter != m_guiImg[house]->end())
    { 
        return m_guiImg[house]->find(ID)->second;
    }
    else
    {
        ImagePtr source = m_guiImg[house]->find(ID)->second;
        ImagePtr copy = source->getRecoloredByHouse(house);
        m_guiImg[house]->insert(std::pair<GuiPic_enum, ImagePtr>(ID, copy));
        return copy;
    }

}

void DataCache::addSoundChunk(Sound_enum ID, Mix_Chunk* tmp){
	soundChunk[ID] = tmp;
}

Mix_Chunk* DataCache::getSoundChunk(Sound_enum ID){
	return soundChunk[ID];
}
Mix_Chunk* DataCache::getChunkFromFile(std::string fileName) {
	Mix_Chunk* returnChunk;
	SDL_RWops* rwop;
	unsigned char * data;

	int len;
	data = ResMan::Instance()->readFile(fileName.c_str(), &len);
	if((rwop = SDL_RWFromMem(data, len)) ==NULL) {
		fprintf(stderr,"DataManager::getChunkFromFile(): Cannot open %s!\n",fileName.c_str());
		exit(EXIT_FAILURE);
	}
	
	if((returnChunk = LoadVOC_RW(rwop, 0)) == NULL) {
		fprintf(stderr,"DataManager::getChunkFromFile(): Cannot load %s!\n",fileName.c_str());
		exit(EXIT_FAILURE);		
	}
	
	SDL_RWclose(rwop);
	return returnChunk;
}

Mix_Chunk* DataCache::concat2Chunks(Mix_Chunk* sound1, Mix_Chunk* sound2)
{
	Mix_Chunk* returnChunk;
	if((returnChunk = (Mix_Chunk*) malloc(sizeof(Mix_Chunk))) == NULL) {
		return NULL;
	}
	
	returnChunk->allocated = 1;
	returnChunk->volume = sound1->volume;
	returnChunk->alen = sound1->alen + sound2->alen;
	
	if((returnChunk->abuf = (Uint8 *)malloc(returnChunk->alen)) == NULL) {
		free(returnChunk);
		return NULL;
	}
	
	memcpy(returnChunk->abuf, sound1->abuf, sound1->alen);
	memcpy(returnChunk->abuf + sound1->alen, sound2->abuf, sound2->alen);

	return returnChunk;
}
/*
std::string	DataCache::getBriefingText(ObjPic_enum mission, ObjPic_enum texttype, int house) {
	return BriefingStrings[0]->getString(0,0);
}

std::string	DataCache::getBriefingText(int i){
	return BriefingStrings[0]->getString(i);
}
*/
Mix_Chunk* DataCache::concat2Chunks(Sound_enum ID1, Sound_enum ID2)
{
	return concat2Chunks(soundChunk[ID1], soundChunk[ID2]);
}

DataCache::~DataCache() {

}
