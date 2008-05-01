#include "DataCache.h" 
#include "pakfile/sound/Vocfile.h"
#include "pakfile/sound/adl/adl.h"
#include <string>
#include <iostream>

DataCache::DataCache() {
}

void DataCache::Init(){
    for (uint8_t i=0; i< NUM_HOUSES; i++)
    {
        m_objImg.push_back(new images());
		m_guiImg.push_back(new images());
    }
	for (uint8_t i = 0; i < MUSIC_RANDOM; i++)
	{
		m_music.push_back(new music());
	}

    int len, maplen;
    uint8_t *data, *mapdata;

    ResMan::Instance()->addRes("ENGLISH");

    addPalette(WESTWOOD_PAL, "INTRO:WESTWOOD.PAL");
	// Not properly decoded yet..
	// CreditsStrings = new Stringfile("ENGLISH:CREDITS.ENG");

    addPalette(INTRO_PAL, "INTRO:INTRO.PAL");	
	IntroStrings = new Stringfile("ENGLISH:INTRO.ENG");

    ResMan::Instance()->addRes("DUNE");

    // FIXME: Something seems to be fscked up with this palette, the Bene Gesserit
    // mentat ends up looking a bit unhealthy greenish, needs to be corrected!
    addPalette(BENE_PAL, "DUNE:BENE.PAL");
    addPalette(IBM_PAL, "DUNE:IBM.PAL");

    ResMan::Instance()->addRes("SOUND");
	addMusic(MUSIC_INTRO, "SOUND:DUNE0.ADL", 4);

    ResMan::Instance()->addRes("ATRE");
	ResMan::Instance()->addRes("GERMAN");
    ResMan::Instance()->addRes("FINALE");
    ResMan::Instance()->addRes("HARK");
    ResMan::Instance()->addRes("HERC");
    ResMan::Instance()->addRes("INTROVOC");
    ResMan::Instance()->addRes("MENTAT");
    ResMan::Instance()->addRes("MERC");
    ResMan::Instance()->addRes("ORDOS");
    ResMan::Instance()->addRes("SCENARIO");
    ResMan::Instance()->addRes("VOC");
    ResMan::Instance()->addRes("XTRE");


    //LOADING FILES    
    data = ResMan::Instance()->readFile("DUNE:UNITS.SHP", &len);
    ShpfilePtr units(new Shpfile(data, len));
    data = ResMan::Instance()->readFile("DUNE:UNITS1.SHP", &len);
    ShpfilePtr units1(new Shpfile(data, len));
    data = ResMan::Instance()->readFile("DUNE:UNITS2.SHP", &len);
    ShpfilePtr units2(new Shpfile(data, len));
	data = ResMan::Instance()->readFile("DUNE:MOUSE.SHP", &len);
	ShpfilePtr mouse(new Shpfile(data, len));
	data = ResMan::Instance()->readFile("DUNE:SHAPES.SHP", &len);
	ShpfilePtr shapes(new Shpfile(data, len));
	data = ResMan::Instance()->readFile("DUNE:ICON.ICN", &len);
	mapdata = ResMan::Instance()->readFile("DUNE:ICON.MAP", &maplen);
	IcnfilePtr icon(new Icnfile(data, len, mapdata, maplen));
	//delete mapdata;

	data = ResMan::Instance()->readFile("DUNE:STATIC.WSA", &len);
	WsafilePtr radar(new Wsafile(data, len));
	data = ResMan::Instance()->readFile("DUNE:MENSHPA.SHP", &len);
	ShpfilePtr menshpa(new Shpfile(data, len));
	data = ResMan::Instance()->readFile("DUNE:MENSHPH.SHP", &len);
	ShpfilePtr menshph(new Shpfile(data, len));
	data = ResMan::Instance()->readFile("DUNE:MENSHPO.SHP", &len);
	ShpfilePtr menshpo(new Shpfile(data, len));
	data = ResMan::Instance()->readFile("DUNE:MENSHPM.SHP", &len);
	ShpfilePtr menshpm(new Shpfile(data, len, getPalette(BENE_PAL)));
	data = ResMan::Instance()->readFile("ENGLISH:CHOAM.ENG", &len);
	ShpfilePtr choam(new Shpfile(data, len));
	data = ResMan::Instance()->readFile("ENGLISH:BTTN.ENG", &len);
	ShpfilePtr bttn(new Shpfile(data, len));
	data = ResMan::Instance()->readFile("FINALE:BIGPLAN.CPS", &len);
	CpsfilePtr bigplan(new Cpsfile(data, len));


	data = ResMan::Instance()->readFile("ENGLISH:HERALD.ENG", &len);
	CpsfilePtr herald(new Cpsfile(data, len));
    data = ResMan::Instance()->readFile("DUNE:MENTATA.CPS", &len);
	CpsfilePtr mentata (new Cpsfile(data, len));
    data = ResMan::Instance()->readFile("DUNE:MENTATO.CPS", &len);
	CpsfilePtr mentato (new Cpsfile(data, len));
    data = ResMan::Instance()->readFile("DUNE:MENTATH.CPS", &len);
	CpsfilePtr mentath (new Cpsfile(data, len));
    data = ResMan::Instance()->readFile("DUNE:MENTATM.CPS", &len);
	CpsfilePtr mentatm (new Cpsfile(data, len, getPalette(BENE_PAL)));
	data = ResMan::Instance()->readFile("ENGLISH:MENTAT.ENG", &len);
	ShpfilePtr mentat (new Shpfile(data, len));

	data = ResMan::Instance()->readFile("DUNE:DUNEMAP.CPS", &len);
	CpsfilePtr dunemap(new Cpsfile(data, len));
	data = ResMan::Instance()->readFile("DUNE:DUNERGN.CPS", &len);
	CpsfilePtr dunergn(new Cpsfile(data, len));
	data = ResMan::Instance()->readFile("DUNE:RGNCLK.CPS", &len);
	CpsfilePtr rgnclk(new Cpsfile(data, len));
	data = ResMan::Instance()->readFile("DUNE:PIECES.SHP", &len);
	ShpfilePtr pieces(new Shpfile(data, len));
	data = ResMan::Instance()->readFile("DUNE:ARROWS.SHP", &len);
	ShpfilePtr arrows(new Shpfile(data, len));

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

	addAnimation(Anim_AtreidesEyes, menshpa->getAnimation(0,4,true), 0.5);
	addAnimation(Anim_AtreidesMouth, menshpa->getAnimation(5,9,true), 5.0);
	addAnimation(Anim_AtreidesShoulder, menshpa->getAnimation(10,10,true), 1.0);
	addAnimation(Anim_AtreidesBook, menshpa->getAnimation(11,12,true), 0.1);
	addAnimation(Anim_HarkonnenEyes, menshph->getAnimation(0,4,true), 0.3);
	addAnimation(Anim_HarkonnenMouth, menshph->getAnimation(5,9,true), 0.5);
	addAnimation(Anim_HarkonnenShoulder, menshph->getAnimation(10,10,true), 1.0);
	addAnimation(Anim_OrdosEyes, menshpo->getAnimation(0,4,true), 0.5);
	addAnimation(Anim_OrdosMouth, menshpo->getAnimation(5,9,true), 5.0);
	addAnimation(Anim_OrdosShoulder, menshpo->getAnimation(10,10,true), 1.0);
	addAnimation(Anim_OrdosRing, menshpo->getAnimation(11,14,true), 6.0);
	addAnimation(Anim_BeneGesseritEyes, menshpm->getAnimation(0,4,true), 0.5);
	addAnimation(Anim_BeneGesseritMouth, menshpm->getAnimation(5,9,true), 5.0);
	addAnimation(Anim_AtreidesPlanet, "MENTAT:FARTR.WSA", 12);
	addAnimation(Anim_HarkonnenPlanet, "MENTAT:FHARK.WSA", 12);
	addAnimation(Anim_OrdosPlanet, "MENTAT:FORDOS.WSA", 12);
	addAnimation(Anim_Win1, "DUNE:WIN1.WSA");
	addAnimation(Anim_Win2, "DUNE:WIN2.WSA");
	addAnimation(Anim_Lose1, "DUNE:LOSTBILD.WSA");
	addAnimation(Anim_Lose2, "DUNE:LOSTVEHC.WSA");
	addAnimation(Anim_Barracks, "MENTAT:BARRAC.WSA");
	addAnimation(Anim_Carryall, "MENTAT:CARRYALL.WSA");
	addAnimation(Anim_ConstructionYard, "MENTAT:CONSTRUC.WSA");
	addAnimation(Anim_Fremen, "MENTAT:FREMEN.WSA");
	addAnimation(Anim_DeathHand, "MENTAT:GOLD-BB.WSA");
	addAnimation(Anim_Devastator, "MENTAT:HARKTANK.WSA");
	addAnimation(Anim_Harvester, "MENTAT:HARVEST.WSA");
	addAnimation(Anim_Radar, "MENTAT:HEADQRTS.WSA");
	addAnimation(Anim_HighTechFactory, "MENTAT:HITCFTRY.WSA");
	addAnimation(Anim_SiegeTank, "MENTAT:HTANK.WSA");
	addAnimation(Anim_HeavyFactory, "MENTAT:HVYFTRY.WSA");
	addAnimation(Anim_Trooper, "MENTAT:HYINFY.WSA");
	addAnimation(Anim_Infantry, "MENTAT:INFANTRY.WSA");
	addAnimation(Anim_IX, "MENTAT:IX.WSA");
	addAnimation(Anim_LightFactory, "MENTAT:LITEFTRY.WSA");
	addAnimation(Anim_Tank, "MENTAT:LTANK.WSA");
	addAnimation(Anim_MCV, "MENTAT:MCV.WSA");
	addAnimation(Anim_Deviator, "MENTAT:ORDRTANK.WSA");
	addAnimation(Anim_Ornithopter, "MENTAT:ORNI.WSA");
	addAnimation(Anim_Raider, "MENTAT:OTRIKE.WSA");
	addAnimation(Anim_Palace, "MENTAT:PALACE.WSA");
	addAnimation(Anim_Quad, "MENTAT:QUAD.WSA");
	addAnimation(Anim_Refinery, "MENTAT:REFINERY.WSA");
	addAnimation(Anim_RepairYard, "MENTAT:REPAIR.WSA");
	addAnimation(Anim_Launcher, "MENTAT:RTANK.WSA");
	addAnimation(Anim_RocketTurret, "MENTAT:RTURRET.WSA");
	addAnimation(Anim_Saboteur, "MENTAT:SABOTURE.WSA");
	addAnimation(Anim_Slab1, "MENTAT:SLAB.WSA");
	addAnimation(Anim_SonicTank, "MENTAT:STANK.WSA");
	addAnimation(Anim_StarPort, "MENTAT:STARPORT.WSA");
	addAnimation(Anim_Silo, "MENTAT:STORAGE.WSA");
	addAnimation(Anim_Trike, "MENTAT:TRIKE.WSA");
	addAnimation(Anim_GunTurret, "MENTAT:TURRET.WSA");
	addAnimation(Anim_Wall, "MENTAT:WALL.WSA");
	addAnimation(Anim_WindTrap, "MENTAT:WINDTRAP.WSA");
	addAnimation(Anim_WOR, "MENTAT:WOR.WSA");
	addAnimation(Anim_Sandworm, "MENTAT:WORM.WSA");
	addAnimation(Anim_Sardaukar, "MENTAT:SARDUKAR.WSA");
	addAnimation(Anim_Frigate, "MENTAT:FRIGATE.WSA");

//	addGuiPic(UI_RadarAnimation, radar->getAnimationAsPictureRow());
	addGuiPic(UI_CursorShape, mouse->getPictureArray(7,1,0|TILE_NORMAL,1|TILE_NORMAL,2|TILE_NORMAL,3|TILE_NORMAL,4|TILE_NORMAL,5|TILE_NORMAL,6|TILE_NORMAL));
	addGuiPic(UI_MouseCursor, mouse->getPicture(0));
//	SDL_SetColorKey(addGuiPic(UI_CursorShape][HOUSE_HARKONNEN], SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
	addGuiPic(UI_CreditsDigits, shapes->getPictureArray(10,1,2|TILE_NORMAL,3|TILE_NORMAL,4|TILE_NORMAL,5|TILE_NORMAL,6|TILE_NORMAL,
																				7|TILE_NORMAL,8|TILE_NORMAL,9|TILE_NORMAL,10|TILE_NORMAL,11|TILE_NORMAL));
//	addGuiPic(UI_GameBar, PicFactory->createGameBar();
	addGuiPic(UI_Indicator, units1->getPictureArray(3,1,8|TILE_NORMAL,9|TILE_NORMAL,10|TILE_NORMAL));
//	addMusic(MUSIC_INTRO, "SOUND:DUNE0.ADL", 2);	

//	SDL_SetColorKey(addGuiPic(UI_Indicator][HOUSE_HARKONNEN], SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
//	addGuiPic(UI_InvalidPlace, PicFactory->createInvalidPlace();
//	addGuiPic(UI_ValidPlace, PicFactory->createValidPlace();
//	addGuiPic(UI_MenuBackground, PicFactory->createMainBackground();
//	addGuiPic(UI_Background, PicFactory->createBackground();
//	addGuiPic(UI_SelectionBox, SDL_LoadBMP_RW(pFileManager->OpenFile("UI_SelectionBox.bmp"),true);
//	SDL_SetColorKey(addGuiPic(UI_SelectionBox][HOUSE_HARKONNEN], SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
//	addGuiPic(UI_TopBar, PicFactory->createTopBar();
	addGuiPic(UI_ButtonUp, choam->getPicture(0));
	addGuiPic(UI_ButtonUp_Pressed, choam->getPicture(1));
	addGuiPic(UI_ButtonDown, choam->getPicture(2));
	addGuiPic(UI_ButtonDown_Pressed, choam->getPicture(3));
//	addGuiPic(UI_MessageBox, PicFactory->createMessageBoxBorder();
	addGuiPic(UI_Mentat, bttn->getPicture(0));
	addGuiPic(UI_Mentat_Pressed, bttn->getPicture(1));
	addGuiPic(UI_Options, bttn->getPicture(2));
	addGuiPic(UI_Options_Pressed, bttn->getPicture(3));
	addGuiPic(UI_Upgrade, choam->getPicture(4));
//	SDL_SetColorKey(addGuiPic(UI_Upgrade][HOUSE_HARKONNEN], SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
	addGuiPic(UI_Upgrade_Pressed, choam->getPicture(5));
//	SDL_SetColorKey(addGuiPic(UI_Upgrade_Pressed][HOUSE_HARKONNEN], SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
//	addGuiPic(UI_Repair, SDL_LoadBMP_RW(pFileManager->OpenFile("Button_Repair.bmp"),true);
//	addGuiPic(UI_Repair_Pressed, SDL_LoadBMP_RW(pFileManager->OpenFile("Button_RepairPushed.bmp"),true);
//	addGuiPic(UI_Difficulty, SDL_LoadBMP_RW(pFileManager->OpenFile("Menu_Difficulty.bmp"),true);
//	SDL_Rect dest1 = { 0,0,addGuiPic(UI_Difficulty][HOUSE_HARKONNEN]->w,30};
//	PicFactory->drawFrame(addGuiPic(UI_Difficulty][HOUSE_HARKONNEN],PictureFactory::DecorationFrame1,&dest1);
//	SDL_SetColorKey(addGuiPic(UI_Difficulty][HOUSE_HARKONNEN], SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
//	addGuiPic(UI_Dif_Easy, SDL_LoadBMP_RW(pFileManager->OpenFile("Difficulty_Easy.bmp"),true);
//	addGuiPic(UI_Dif_Hard, SDL_LoadBMP_RW(pFileManager->OpenFile("Difficulty_Hard.bmp"),true);
//	addGuiPic(UI_Dif_Medium, SDL_LoadBMP_RW(pFileManager->OpenFile("Difficulty_Medium.bmp"),true);
//	addGuiPic(UI_Minus, SDL_LoadBMP_RW(pFileManager->OpenFile("Button_Minus.bmp"),true);
//	addGuiPic(UI_Minus_Pressed, SDL_LoadBMP_RW(pFileManager->OpenFile("Button_MinusPushed.bmp"),true);
//	addGuiPic(UI_Plus,  SDL_LoadBMP_RW(pFileManager->OpenFile("Button_Plus.bmp"),true);
//	addGuiPic(UI_Plus_Pressed, SDL_LoadBMP_RW(pFileManager->OpenFile("Button_PlusPushed.bmp"),true);
//	addGuiPic(UI_MissionSelect, SDL_LoadBMP_RW(pFileManager->OpenFile("Menu_MissionSelect.bmp"),true);
//	PicFactory->drawFrame(addGuiPic(UI_MissionSelect][HOUSE_HARKONNEN],PictureFactory::SimpleFrame,NULL);
//	SDL_SetColorKey(addGuiPic(UI_MissionSelect][HOUSE_HARKONNEN], SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
//	addGuiPic(UI_OptionsMenu, PicFactory->createOptionsMenu();
//	SDL_Surface* tmp;
//	if((tmp = SDL_CreateRGBSurface(SDL_HWSURFACE,192,27,8,0,0,0,0)) == NULL) {
//		fprintf(stderr,"DataManager::DataManager(): Cannot create surface!\n");
//		exit(EXIT_FAILURE);	
//	}
//	SDL_SetColors(tmp, palette->colors, 0, palette->ncolors);
//	SDL_FillRect(tmp,NULL,133);
//	addGuiPic(UI_LoadSaveWindow, PicFactory->createMenu(tmp,208);
//	SDL_FreeSurface(tmp);
//	addGuiPic(UI_DuneLegacy, SDL_LoadBMP_RW(pFileManager->OpenFile("DuneLegacy.bmp"),true);
//	addGuiPic(UI_GameMenu, PicFactory->createMenu(addGuiPic(UI_DuneLegacy][HOUSE_HARKONNEN],158);
//	PicFactory->drawFrame(addGuiPic(UI_DuneLegacy][HOUSE_HARKONNEN],PictureFactory::SimpleFrame);

	addGuiPic(UI_PlanetBackground, bigplan->getPicture());
//	PicFactory->drawFrame(addGuiPic(UI_PlanetBackground, PictureFactory::SimpleFrame);
//	addGuiPic(UI_MenuButtonBorder, PicFactory->createFrame(PictureFactory::DecorationFrame1,190,123,false);

//	PicFactory->drawFrame(addGuiPic(UI_DuneLegacy, PictureFactory::SimpleFrame);

	addGuiPic(UI_MentatBackground, mentata->getPicture(), HOUSE_ATREIDES);
	addGuiPic(UI_MentatBackground, mentata->getPicture(), HOUSE_FREMEN);
	addGuiPic(UI_MentatBackground, mentato->getPicture(), HOUSE_ORDOS);
	addGuiPic(UI_MentatBackground, mentato->getPicture(), HOUSE_MERCENARY);
	addGuiPic(UI_MentatBackground, mentath->getPicture(), HOUSE_HARKONNEN);
	addGuiPic(UI_MentatBackground, mentatm->getPicture(), HOUSE_SARDAUKAR);
	addGuiPic(UI_MentatYes, mentat->getPicture(0));
	addGuiPic(UI_MentatYes_Pressed, mentat->getPicture(1));
	addGuiPic(UI_MentatNo, mentat->getPicture(2));
	addGuiPic(UI_MentatNo_Pressed, mentat->getPicture(3));
	addGuiPic(UI_MentatExit,mentat->getPicture(4));
	addGuiPic(UI_MentatExit_Pressed, mentat->getPicture(5));
	addGuiPic(UI_MentatProcced, mentat->getPicture(6));
	addGuiPic(UI_MentatProcced_Pressed, mentat->getPicture(7));
	addGuiPic(UI_MentatRepeat, mentat->getPicture(8));
	addGuiPic(UI_MentatRepeat_Pressed, mentat->getPicture(9));
	addGuiPic(UI_HouseChoiceBackground, herald->getPicture());
//	addGuiPic(UI_HouseSelect, PicFactory->createHouseSelect(addGuiPic(UI_HouseChoiceBackground][HOUSE_HARKONNEN]);
//	addGuiPic(UI_HeraldAtre_Coloured, GetSubPicture(addGuiPic(UI_HouseChoiceBackground][HOUSE_HARKONNEN],20,54,83,91);
//	addGuiPic(UI_HeraldOrd_Coloured, GetSubPicture(addGuiPic(UI_HouseChoiceBackground][HOUSE_HARKONNEN],117,54,83,91);
//	addGuiPic(UI_HeraldHark_Coloured, GetSubPicture(addGuiPic(UI_HouseChoiceBackground][HOUSE_HARKONNEN],215,54,82,91);

//	addGuiPic(UI_MapChoiceScreen,[HOUSE_ATREIDES] = PicFactory->createMapChoiceScreen(HOUSE_ATREIDES);
//	addGuiPic(UI_MapChoiceScreen,[HOUSE_ORDOS] = PicFactory->createMapChoiceScreen(HOUSE_ORDOS);
//	addGuiPic(UI_MapChoiceScreen,[HOUSE_HARKONNEN] = PicFactory->createMapChoiceScreen(HOUSE_HARKONNEN);
	addGuiPic(UI_MapChoiceMapOnly,dunemap->getPicture());
//	SDL_SetColorKey(m_guiImgUIGraphic,UI_MapChoiceMapOnly][HOUSE_HARKONNEN], SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
	addGuiPic(UI_MapChoiceMap, dunergn->getPicture());
//	SDL_SetColorKey(UIGraphic,UI_MapChoiceMap][HOUSE_HARKONNEN], SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
	addGuiPic(UI_MapChoiceClickMap, rgnclk->getPicture());

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

	BriefingStrings[0] = new Stringfile("ENGLISH:TEXTA.ENG");
	BriefingStrings[1] = new Stringfile("ENGLISH:TEXTO.ENG");
	BriefingStrings[2] = new Stringfile("ENGLISH:TEXTH.ENG");

	addMusic(MUSIC_INTRO, "SOUND:DUNE0.ADL", 2);	
	addMusic(MUSIC_LOSE, "SOUND:DUNE1.ADL", 3);
	addMusic(MUSIC_PEACE, "SOUND:DUNE2.ADL", 6);
	addMusic(MUSIC_PEACE, "SOUND:DUNE3.ADL", 6);
	addMusic(MUSIC_PEACE, "SOUND:DUNE4.ADL", 6);
	addMusic(MUSIC_PEACE, "SOUND:DUNE5.ADL", 6);
	addMusic(MUSIC_PEACE, "SOUND:DUNE6.ADL", 6);
	addMusic(MUSIC_PEACE, "SOUND:DUNE9.ADL", 4);
	addMusic(MUSIC_PEACE, "SOUND:DUNE10.ADL", 2);
	addMusic(MUSIC_ATTACK, "SOUND:DUNE11.ADL", 7);
	addMusic(MUSIC_ATTACK, "SOUND:DUNE12.ADL", 7);
	addMusic(MUSIC_ATTACK, "SOUND:DUNE13.ADL", 7);
	addMusic(MUSIC_ATTACK, "SOUND:DUNE14.ADL", 7);
	addMusic(MUSIC_ATTACK, "SOUND:DUNE15.ADL", 7);
	addMusic(MUSIC_PEACE, "SOUND:DUNE18.ADL", 6);
	addMusic(MUSIC_PEACE, "SOUND:DUNE19.ADL", 4);
	addMusic(MUSIC_WIN, "SOUND:DUNE20.ADL", 2);

	delete data;

}

void DataCache::addPalette(Palette_enum palette, std::string paletteFile)
{
    int len;
    uint8_t * data = ResMan::Instance()->readFile(paletteFile, &len);
    PalettefilePtr tmp (new Palettefile(data, len));

    m_palette[palette] = tmp; //pal;
}

SDL_Palette* DataCache::getPalette(Palette_enum palette)
{
#ifdef THREADS
    spinlock:
    if(!m_palette[palette])
        goto spinlock;
#endif
    return m_palette[palette]->getPalette();
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

    images::iterator iter;
#ifdef THREADS
	spinlock:
#endif
	iter = m_objImg[house]->find(ID);
    if (iter != m_objImg[house]->end())
    { 
        return m_objImg[house]->find(ID)->second;
    }
    else
    {
#ifdef THREADS
		// If house is harkonnen we know that there shouldn't be any other
		// to look for and therefore must be that graphic hasn't been loaded yet.
		if (house == HOUSE_HARKONNEN)
			goto spinlock;
#endif
        ImagePtr source = m_objImg[house]->find(ID)->second;
        ImagePtr copy = source->getRecoloredByHouse(house);
        m_objImg[house]->insert(std::pair<ObjPic_enum, ImagePtr>(ID, copy));
        return copy;
    }

}

ImagePtr DataCache::getGuiPic(GuiPic_enum ID, HOUSETYPE house) {

    images::iterator iter;
#ifdef THREADS
	spinlock:
#endif
	iter = m_guiImg[house]->find(ID);
    if (iter != m_guiImg[house]->end())
    { 
        return m_guiImg[house]->find(ID)->second;
    }
    else
    {
#ifdef THREADS
		if (house == HOUSE_HARKONNEN)
			goto spinlock;
#endif
        ImagePtr source = m_guiImg[house]->find(ID)->second;
        ImagePtr copy = source->getRecoloredByHouse(house);
        m_guiImg[house]->insert(std::pair<GuiPic_enum, ImagePtr>(ID, copy));
        return copy;
    }

}

void DataCache::addSoundChunk(Sound_enum ID, Mix_Chunk* tmp){
	soundChunk[ID] = tmp;
}

void DataCache::addMusic(MUSICTYPE musicType, std::string filename, uint16_t trackNum)
{
	songFiles[musicType].push_back(songFile(filename, trackNum));
#ifdef THREADS
	addMusic(musicType, songFiles[musicType].size()-1);
#endif
}

Mix_Chunk* DataCache::addMusic(MUSICTYPE musicType, uint16_t ID)
{
	songFile song = songFiles[musicType][ID];
	int len;
	uint8_t * data = ResMan::Instance()->readFile(song.first, &len);
	SDL_RWops* test = SDL_RWFromMem(data, len);
	CadlPlayer *p = new CadlPlayer(test);
	Mix_Chunk* tmp = p->getUpsampledSubsong(song.second, 22050, AUDIO_S16LSB, 1);
	SDL_RWclose(test);
    m_music[musicType]->insert(std::pair<uint16_t, Mix_Chunk*>(ID, tmp));
	delete data;
	delete p;
	return tmp;
}

// Searches through list of loaded songs to see if the specific song is loaded.
// If loaded, it will return the song, if not it will load and return the song.
Mix_Chunk* DataCache::getMusic(MUSICTYPE musicType, uint16_t ID)
{
    music::iterator iter;
#ifdef THREADS
	spinlock:
#endif
	iter = m_music[musicType]->find(ID);
    if (iter != m_music[musicType]->end())
    {
        return m_music[musicType]->find(ID)->second;
    }
    else
    {
#ifdef	THREADS
		goto spinlock;
#endif
		return addMusic(musicType, ID);
    }
}

Mix_Chunk* DataCache::getSoundChunk(Sound_enum ID){
	return soundChunk[ID];
}

Mix_Chunk* DataCache::getChunkFromFile(std::string fileName) {
	Mix_Chunk* returnChunk;
	SDL_RWops* rwop;
	int len;
	uint8_t * data;

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
	delete data;
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

std::string	DataCache::getBriefingText(uint16_t mission, uint16_t textType, HOUSETYPE house) {
#ifdef THREADS
	spinlock:
	if(!BriefingStrings[house])
		goto spinlock;
#endif
	return BriefingStrings[house]->getString(mission,textType);
}

std::string	DataCache::getIntroString(uint16_t i){
#ifdef THREADS
	spinlock:
	if(!IntroStrings)
		goto spinlock;
#endif
	return IntroStrings->getString(i);
}

std::string	DataCache::getCreditsString(uint16_t i){
#ifdef THREADS
	spinlock:
	if(!IntroStrings)
		goto spinlock;
#endif
	return CreditsStrings->getString(i);
}

void DataCache::addAnimation(Animation_enum ID, std::string fileName, double frameRate) {
	int len;
    uint8_t * data = ResMan::Instance()->readFile(fileName, &len);
    WsafilePtr wsafile(new Wsafile(data, len));
    SDL_Palette* palette = getPalette(IBM_PAL);
	
	Animation* animation = wsafile->getAnimation(0,wsafile->getNumFrames() - 1, palette, false);
	if(frameRate)
		animation->setFrameRate(frameRate);
	Anim[ID] = animation;
	delete data;
}

void DataCache::addAnimation(Animation_enum ID, Animation* animation, double frameRate) {
	if(frameRate)
		animation->setFrameRate(frameRate);
	Anim[ID] = animation;
}

Animation* DataCache::getAnimation(Animation_enum id) {
	if(id >= NUM_ANIMATION) {
		fprintf(stderr,"DataManager::getAnimation(): Animation with id %d is not available!\n",id);
		exit(EXIT_FAILURE);
	}
	
	return Anim[id];
}

Mix_Chunk* DataCache::concat2Chunks(Sound_enum ID1, Sound_enum ID2)
{
	return concat2Chunks(soundChunk[ID1], soundChunk[ID2]);
}

DataCache::~DataCache() {

}
