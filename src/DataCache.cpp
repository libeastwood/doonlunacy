#include "DataCache.h" 
#include "ResMan.h"
#include "Log.h"

#include "GCObject.h"

#include "pakfile/Cpsfile.h"
#include "pakfile/Icnfile.h"
#include "pakfile/Shpfile.h"
#include "pakfile/Wsafile.h"

#include <PalFile.h>
#include <StringFile.h>
#include <VocFile.h>

#include <string>

#include <boost/format.hpp>

using namespace libconfig;

DataCache::DataCache() {
}

void DataCache::Init(){
    //TODO:Should use ResMan.
    m_dataConfig = new Config();

    try
    {
        m_dataConfig->readFile("data.dunetxt");
    }
    catch(ParseException& ex)
    {
        LOG_FATAL("DataCache", "Fatal error loading configuration file on line %d: %s", 
            ex.getLine(), ex.getError());

        exit(EXIT_FAILURE);
    }


    for (uint8_t i=0; i< NUM_HOUSES; i++)
    {
        m_objImg.push_back(new images);
		m_guiImg.push_back(new images);
    }

	soundChunk.resize(NUM_SOUNDCHUNK);
    int len, maplen;
    uint8_t *data, *mapdata;
//    Image *tmp;

    ResMan::Instance()->addRes("ENGLISH");

    addPalette(WESTWOOD_PAL, "INTRO:WESTWOOD.PAL");
	// Not properly decoded yet..
	// CreditsStrings = new StringFile("ENGLISH:CREDITS.ENG");

    addPalette(INTRO_PAL, "INTRO:INTRO.PAL");
    data = ResMan::Instance()->readFile("ENGLISH:INTRO.ENG", &len);
    IntroStrings = new StringFile(data);

    ResMan::Instance()->addRes("DUNE");

    addPalette(BENE_PAL, "DUNE:BENE.PAL");
    addPalette(IBM_PAL, "DUNE:IBM.PAL");

    ResMan::Instance()->addRes("SOUND");

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
	IcnfilePtr  icon(new Icnfile(data, len, mapdata, maplen));
	//delete mapdata;

//	data = ResMan::Instance()->readFile("DUNE:STATIC.WSA", &len);
//	WsafilePtr radar(new Wsafile(data, len));
	data = ResMan::Instance()->readFile("DUNE:MENSHPA.SHP", &len);
	ShpfilePtr menshpa(new Shpfile(data, len));
	data = ResMan::Instance()->readFile("DUNE:MENSHPH.SHP", &len);
	ShpfilePtr menshph(new Shpfile(data, len));
	data = ResMan::Instance()->readFile("DUNE:MENSHPO.SHP", &len);
	ShpfilePtr menshpo(new Shpfile(data, len));
	data = ResMan::Instance()->readFile("DUNE:MENSHPM.SHP", &len);
	ShpfilePtr menshpm(new Shpfile(data, len, getPalette(BENE_PAL)));
//	data = ResMan::Instance()->readFile("ENGLISH:CHOAM.ENG", &len);
//	ShpfilePtr choam(new Shpfile(data, len));
//	data = ResMan::Instance()->readFile("ENGLISH:BTTN.ENG", &len);
//	ShpfilePtr bttn(new Shpfile(data, len));
//	data = ResMan::Instance()->readFile("FINALE:BIGPLAN.CPS", &len);
//	CpsfilePtr bigplan(new Cpsfile(data, len));


//	data = ResMan::Instance()->readFile("ENGLISH:HERALD.ENG", &len);
//	CpsfilePtr herald(new Cpsfile(data, len));
    data = ResMan::Instance()->readFile("DUNE:MENTATA.CPS", &len);
	CpsfilePtr mentata (new Cpsfile(data, len));
    data = ResMan::Instance()->readFile("DUNE:MENTATO.CPS", &len);
	CpsfilePtr mentato (new Cpsfile(data, len));
    data = ResMan::Instance()->readFile("DUNE:MENTATH.CPS", &len);
	CpsfilePtr mentath (new Cpsfile(data, len));
    data = ResMan::Instance()->readFile("DUNE:MENTATM.CPS", &len);
	CpsfilePtr mentatm (new Cpsfile(data, len, getPalette(BENE_PAL)));
//	data = ResMan::Instance()->readFile("ENGLISH:MENTAT.ENG", &len);
//	ShpfilePtr mentat (new Shpfile(data, len));

//	data = ResMan::Instance()->readFile("DUNE:DUNEMAP.CPS", &len);
//	CpsfilePtr dunemap(new Cpsfile(data, len));
//	data = ResMan::Instance()->readFile("DUNE:DUNERGN.CPS", &len);
//	CpsfilePtr dunergn(new Cpsfile(data, len));
//	data = ResMan::Instance()->readFile("DUNE:RGNCLK.CPS", &len);
//	CpsfilePtr rgnclk(new Cpsfile(data, len));
//	data = ResMan::Instance()->readFile("DUNE:PIECES.SHP", &len);
//	ShpfilePtr pieces(new Shpfile(data, len));
//	data = ResMan::Instance()->readFile("DUNE:ARROWS.SHP", &len);
//	ShpfilePtr arrows(new Shpfile(data, len));
//    data = ResMan::Instance()->readFile("DUNE:FAME.CPS", &len);
//    CpsfilePtr fame(new Cpsfile(data, len));
//    data = ResMan::Instance()->readFile("DUNE:MAPMACH.CPS", &len);
//    CpsfilePtr mapmach(new Cpsfile(data, len));
//    data = ResMan::Instance()->readFile("DUNE:SCREEN.CPS", &len);
//    CpsfilePtr screen(new Cpsfile(data, len));

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
	addObjPic(ObjPic_WindTrap, icon->getPictureArray(19));
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
	addObjPic(ObjPic_StarPort, icon->getPictureArray(20));
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

//	addGuiPic(UI_RadarAnimation, radar->getAnimationAsPictureRow());
	addGuiPic(UI_MouseCursor, mouse->getPictureArray(7,1,0|TILE_NORMAL,1|TILE_NORMAL,2|TILE_NORMAL,3|TILE_NORMAL,4|TILE_NORMAL,5|TILE_NORMAL,6|TILE_NORMAL));
//	addGuiPic(UI_MouseCursor, mouse->getPicture(0));
//	SDL_SetColorKey(addGuiPic(UI_CursorShape][HOUSE_HARKONNEN], SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
//	addGuiPic(UI_CreditsDigits, shapes->getPictureArray(10,1,2|TILE_NORMAL,3|TILE_NORMAL,4|TILE_NORMAL,5|TILE_NORMAL,6|TILE_NORMAL,
//																				7|TILE_NORMAL,8|TILE_NORMAL,9|TILE_NORMAL,10|TILE_NORMAL,11|TILE_NORMAL));
//	addGuiPic(UI_GameBar, PicFactory->createGameBar();
//	addGuiPic(UI_Indicator, units1->getPictureArray(3,1,8|TILE_NORMAL,9|TILE_NORMAL,10|TILE_NORMAL));

//	SDL_SetColorKey(addGuiPic(UI_Indicator][HOUSE_HARKONNEN], SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
//	addGuiPic(UI_InvalidPlace, PicFactory->createInvalidPlace();
//	addGuiPic(UI_ValidPlace, PicFactory->createValidPlace();
//	addGuiPic(UI_MenuBackground, PicFactory->createMainBackground();
//	addGuiPic(UI_Background, PicFactory->createBackground();
    
//	addGuiPic(UI_SelectionBox, mouse->getPicture(6));
//	SDL_SetColorKey(addGuiPic(UI_SelectionBox][HOUSE_HARKONNEN], SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
//	addGuiPic(UI_TopBar, PicFactory->createTopBar();
//	addGuiPic(UI_ButtonUp, choam->getPicture(0));
//	addGuiPic(UI_ButtonUp_Pressed, choam->getPicture(1));
//	addGuiPic(UI_ButtonDown, choam->getPicture(2));
//	addGuiPic(UI_ButtonDown_Pressed, choam->getPicture(3));
//	addGuiPic(UI_MessageBox, PicFactory->createMessageBoxBorder();
//	addGuiPic(UI_Mentat, bttn->getPicture(0));
//	addGuiPic(UI_Mentat_Pressed, bttn->getPicture(1));
//	addGuiPic(UI_Options, bttn->getPicture(2));
//	addGuiPic(UI_Options_Pressed, bttn->getPicture(3));
//	addGuiPic(UI_Upgrade, choam->getPicture(4));
//	SDL_SetColorKey(addGuiPic(UI_Upgrade][HOUSE_HARKONNEN], SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
//	addGuiPic(UI_Upgrade_Pressed, choam->getPicture(5));
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

//	addGuiPic(UI_PlanetBackground, bigplan->getPicture());
//	PicFactory->drawFrame(addGuiPic(UI_PlanetBackground, PictureFactory::SimpleFrame);
//	addGuiPic(UI_MenuButtonBorder, PicFactory->createFrame(PictureFactory::DecorationFrame1,190,123,false);

//	PicFactory->drawFrame(addGuiPic(UI_DuneLegacy, PictureFactory::SimpleFrame);

	addGuiPic(UI_MentatBackground, mentata->getPicture(), HOUSE_ATREIDES);
	addGuiPic(UI_MentatBackground, mentata->getPicture(), HOUSE_FREMEN);
	addGuiPic(UI_MentatBackground, mentato->getPicture(), HOUSE_ORDOS);
	addGuiPic(UI_MentatBackground, mentato->getPicture(), HOUSE_MERCENARY);
	addGuiPic(UI_MentatBackground, mentath->getPicture(), HOUSE_HARKONNEN);
	addGuiPic(UI_MentatBackground, mentatm->getPicture(), HOUSE_SARDAUKAR);
	/*addGuiPic(UI_MentatYes, mentat->getPicture(0));
	addGuiPic(UI_MentatYes_Pressed, mentat->getPicture(1));
	addGuiPic(UI_MentatNo, mentat->getPicture(2));
	addGuiPic(UI_MentatNo_Pressed, mentat->getPicture(3));
	addGuiPic(UI_MentatExit,mentat->getPicture(4));
	addGuiPic(UI_MentatExit_Pressed, mentat->getPicture(5));
	addGuiPic(UI_MentatProcced, mentat->getPicture(6));
	addGuiPic(UI_MentatProcced_Pressed, mentat->getPicture(7));
	addGuiPic(UI_MentatRepeat, mentat->getPicture(8));
	addGuiPic(UI_MentatRepeat_Pressed, mentat->getPicture(9));*/
//	addGuiPic(UI_HouseChoiceBackground, herald->getPicture());
//	addGuiPic(UI_HouseSelect, PicFactory->createHouseSelect(addGuiPic(UI_HouseChoiceBackground][HOUSE_HARKONNEN]);
//	addGuiPic(UI_HeraldAtre_Coloured, GetSubPicture(addGuiPic(UI_HouseChoiceBackground][HOUSE_HARKONNEN],20,54,83,91);
//	addGuiPic(UI_HeraldOrd_Coloured, GetSubPicture(addGuiPic(UI_HouseChoiceBackground][HOUSE_HARKONNEN],117,54,83,91);
//	addGuiPic(UI_HeraldHark_Coloured, GetSubPicture(addGuiPic(UI_HouseChoiceBackground][HOUSE_HARKONNEN],215,54,82,91);

//	addGuiPic(UI_MapChoiceScreen,[HOUSE_ATREIDES] = PicFactory->createMapChoiceScreen(HOUSE_ATREIDES);
//	addGuiPic(UI_MapChoiceScreen,[HOUSE_ORDOS] = PicFactory->createMapChoiceScreen(HOUSE_ORDOS);
//	addGuiPic(UI_MapChoiceScreen,[HOUSE_HARKONNEN] = PicFactory->createMapChoiceScreen(HOUSE_HARKONNEN);
//	addGuiPic(UI_MapChoiceMapOnly,dunemap->getPicture());
//	SDL_SetColorKey(m_guiImgUIGraphic,UI_MapChoiceMapOnly][HOUSE_HARKONNEN], SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
//	addGuiPic(UI_MapChoiceMap, dunergn->getPicture());
//	SDL_SetColorKey(UIGraphic,UI_MapChoiceMap][HOUSE_HARKONNEN], SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
//	addGuiPic(UI_MapChoiceClickMap, rgnclk->getPicture());

/*    addGuiPic(UI_TopBorder, fame->getPicture()->getPictureCrop(Rect(12, 73, 1, 4)));
    addGuiPic(UI_BottomBorder, fame->getPicture()->getPictureCrop(Rect(12, 72, 1, 4)));
    addGuiPic(UI_LeftBorder, fame->getPicture()->getPictureCrop(Rect(1, 83, 4, 1)));
    addGuiPic(UI_RightBorder, fame->getPicture()->getPictureCrop(Rect(1, 83, 4, 1)));*/
/*
    tmp = fame->getPicture()->getPictureCrop(Rect(64, 9, 8, 8));
    tmp->setColorKey();
    tmp->putPixel(UPoint(6,7), 0); // Edge smoothing..
    tmp->putPixel(UPoint(7,6), 0);
    tmp->putPixel(UPoint(7,7), 0);
    addGuiPic(UI_Corner1NW, tmp);

    tmp = fame->getPicture()->getPictureCrop(Rect(248, 9, 8, 8));
    tmp->setColorKey();
    tmp->fillRect(0, Rect(0, 5, 1, 2));
    tmp->fillRect(0, Rect(0, 6, 2, 2));
    addGuiPic(UI_Corner1NE, tmp);
    
    tmp = fame->getPicture()->getPictureCrop(Rect(64, 23, 8, 8));
    tmp->setColorKey();
    tmp->putPixel(UPoint(6,0), 0);
    tmp->putPixel(UPoint(7,0), 0);
    tmp->putPixel(UPoint(7,1), 0);
    addGuiPic(UI_Corner1SW, tmp);

    tmp = fame->getPicture()->getPictureCrop(Rect(248, 23, 8, 8));
    tmp->setColorKey();
    tmp->putPixel(UPoint(0, 0), 0);
    tmp->putPixel(UPoint(0, 1), 0);
    tmp->putPixel(UPoint(1, 0), 0);
    addGuiPic(UI_Corner1SE, tmp);

    tmp = fame->getPicture()->getPictureCrop(Rect(10, 137, 12, 12));
    tmp->setColorKey();
    tmp->drawHLine(UPoint(8, 11), 11, 0);
    tmp->drawHLine(UPoint(10, 9), 11, 0);
    tmp->drawVLine(UPoint(11, 11), 4, 0);
    addGuiPic(UI_Corner2NW, tmp);

    tmp = fame->getPicture()->getPictureCrop(Rect(51, 137, 12, 12));
    tmp->setColorKey();
    tmp->drawVLine(UPoint(0, 4), 11, 0); // Edge smoothing..
    tmp->drawVLine(UPoint(1, 9), 11, 0);
    tmp->drawVLine(UPoint(2, 10), 11, 0);
    tmp->drawHLine(UPoint(2, 11), 6, 0);
    addGuiPic(UI_Corner2NE, tmp);

    tmp = fame->getPicture()->getPictureCrop(Rect(10, 180, 10, 11));
    tmp->setColorKey();
    tmp->drawHLine(UPoint(2, 0), 9, 0); // Edge smoothing..
    tmp->drawHLine(UPoint(6, 1), 9, 0);
    tmp->drawVLine(UPoint(7, 2), 9, 0);
    tmp->drawVLine(UPoint(8, 3), 4, 0);
    tmp->drawVLine(UPoint(9, 3), 7, 0);
    addGuiPic(UI_Corner2SW, tmp);

    tmp = fame->getPicture()->getPictureCrop(Rect(52, 180, 11, 11));
    tmp->setColorKey();
    tmp->drawVLine(UPoint(0, 0), 7, 0); // Edge smoothing..
    tmp->drawVLine(UPoint(1, 0), 3, 0);
    tmp->drawVLine(UPoint(2, 0), 2, 0);
    tmp->drawVLine(UPoint(3, 0), 1, 0);
    tmp->drawHLine(UPoint(0, 0), 7, 0);
    addGuiPic(UI_Corner2SE, tmp);

    tmp = fame->getPicture()->getPictureCrop(Rect(1, 73, 12, 12));
    tmp->setColorKey();
    tmp->fillRect(0, Rect(6, 6, 5, 5));
    addGuiPic(UI_Corner3NW, tmp);

    tmp = fame->getPicture()->getPictureCrop(Rect(308, 72, 12, 12));
    tmp->setColorKey();
    tmp->fillRect(0, Rect(0, 7, 5, 5));
    addGuiPic(UI_Corner3NE, tmp);
    
    tmp = fame->getPicture()->getPictureCrop(Rect(0, 188, 12, 12));
    tmp->setColorKey();
    tmp->fillRect(0, Rect(7, 0, 5, 5));
    addGuiPic(UI_Corner3SW, tmp);

    tmp = fame->getPicture()->getPictureCrop(Rect(308, 188, 12, 12));
    tmp->fillRect(0, Rect(0, 0, 5, 5));
    tmp->setColorKey();
    addGuiPic(UI_Corner3SE, tmp);
*/
//    addGuiPic(UI_BlankFiller, mapmach->getPicture()->getPictureCrop(Rect(60, 184, 150, 16)));

    // Ugly, needs to be improved..
//    addGuiPic(UI_MenuBackground, fame->getPicture()->getPictureCrop(Rect(0, 0, 50, 50)));

//    addGuiPic(UI_Mentat_HeraldHarkonnen, fame->getPicture()->getPictureCrop(Rect(9, 136, 56, 56)));
//    addGuiPic(UI_Mentat_HeraldAtreides, fame->getPicture()->getPictureCrop(Rect(65, 136, 56, 56)));
//    addGuiPic(UI_Mentat_HeraldOrdos, fame->getPicture()->getPictureCrop(Rect(121, 136, 56, 56)));



	data = ResMan::Instance()->readFile("ENGLISH:TEXTA.ENG", &len);	
	BriefingStrings[0] = new StringFile(data);
	data = ResMan::Instance()->readFile("ENGLISH:TEXTO.ENG", &len);	
	BriefingStrings[1] = new StringFile(data);
	data = ResMan::Instance()->readFile("ENGLISH:TEXTH.ENG", &len);	
	BriefingStrings[2] = new StringFile(data);
}

void DataCache::addPalette(Palette_enum palette, std::string paletteFile)
{
    int len;
    uint8_t * data = ResMan::Instance()->readFile(paletteFile, &len);
    PalfilePtr tmp (new PalFile(data, len));

    m_palette[palette] = tmp; //pal;
    m_palStrings.insert(std::pair<std::string, PalfilePtr>(paletteFile, tmp));
}

SDL_Palette* DataCache::getPalette(Palette_enum palette)
{
    return m_palette[palette]->getPalette();
}

SDL_Palette* DataCache::getPalette(std::string paletteFile)
{
    return m_palStrings[paletteFile]->getPalette();
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
	iter = m_objImg[house]->find(ID);
    if (iter != m_objImg[house]->end())
    { 
        return m_objImg[house]->find(ID)->second;
    }
    else
    {
        ImagePtr source = m_objImg[HOUSE_HARKONNEN]->find(ID)->second;
        ImagePtr copy = source->getRecoloredByHouse(house);
        m_objImg[house]->insert(std::pair<ObjPic_enum, ImagePtr>(ID, copy));
        return copy;
    }

}

ImagePtr DataCache::getGuiPic(GuiPic_enum ID, HOUSETYPE house) {

    images::iterator iter;
	iter = m_guiImg[house]->find(ID);
    if (iter != m_guiImg[house]->end())
    { 
        return m_guiImg[house]->find(ID)->second;
    }
    else
    {
        ImagePtr source = m_guiImg[HOUSE_HARKONNEN]->find(ID)->second;
        ImagePtr copy = source->getRecoloredByHouse(house);
        m_guiImg[house]->insert(std::pair<GuiPic_enum, ImagePtr>(ID, copy));
        return copy;
    }

}

song * DataCache::getMusic(MUSICTYPE musicType, uint16_t ID)
{
  
    Setting& node = m_dataConfig->lookup("music");
    song * newSong = new song;

    std::string filename = node[musicType][ID][0];
    int track = (int)node[musicType][ID][1];

    newSong->filename = filename;
    newSong->track = track;

    return newSong;
}


Mix_Chunk* DataCache::getSoundChunk(std::string ID)
{

    std::string fullpath = "sounds.";
    fullpath+=ID;

    std::string fileName;

	Mix_Chunk* returnChunk;
	
    try
    {
        Setting& node = m_dataConfig->lookup(fullpath);
        node.lookupValue("filename", fileName);
        SDL_RWops* rwop;
        int len;
        uint8_t * data;

        data = ResMan::Instance()->readFile(fileName.c_str(), &len);
        if((rwop = SDL_RWFromMem(data, len)) ==NULL) 
        {
            LOG_ERROR("DataCache", "getChunkFromFile(): Cannot open %s!",fileName.c_str());
            exit(EXIT_FAILURE);
        }

        if((returnChunk = LoadVOC_RW(rwop, 0)) == NULL) 
        {
            LOG_ERROR("DataCache", "getChunkFromFile(): Cannot load %s!",fileName.c_str());
            exit(EXIT_FAILURE);		
        }

        SDL_RWclose(rwop);
        free(data);

    }
    catch(ParseException& ex)
    {
        LOG_FATAL("DataCache", "Setting not found %d: %s", 
            ex.getLine(), ex.getError());
    }

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
	return BriefingStrings[house]->getString(mission,textType);
}

std::string	DataCache::getIntroString(uint16_t i){
	return IntroStrings->getString(i);
}

std::string	DataCache::getCreditsString(uint16_t i){
	return CreditsStrings->getString(i);
}

libconfig::Config *DataCache::getConfig()
{
	return m_dataConfig;
}

GCObject *DataCache::getGCObject(std::string path)
{
	GCObject *gcObj = NULL;
	if(!m_gcObjs.empty())
		for(uint32_t i = 0; i < m_gcObjs.size(); i++)
			if(m_gcObjs[i]->getPath() == path)
			{
				gcObj = m_gcObjs[i];
			}
	if(gcObj == NULL){
		gcObj = new GCObject(path);
		m_gcObjs.push_back(gcObj);
	}
	return gcObj;

}

void DataCache::freeGCObjects()
{
	for(uint32_t i = 0; i < m_gcObjs.size(); i++)
		m_gcObjs[i]->freeIfUnique();
}

Animation *DataCache::getAnimation(std::string path)
{
    std::string fullpath = "animations.";
    fullpath+=path;

    Animation* animation = NULL;
    
    try
    {
        Setting& node = m_dataConfig->lookup(fullpath);
		
		int len;
		uint8_t *data;

        std::string fileName;
		
		SDL_Palette* palette;
        if(node.lookupValue("palette", fileName))
		{
			data = ResMan::Instance()->readFile(fileName, &len);
			PalfilePtr tmp(new PalFile(data, len));
			palette = getPalette(fileName);
		}
		else
			palette = getPalette(IBM_PAL);

        node.lookupValue("filename", fileName);
        std::string type = fileName.substr(fileName.length()-3, 3);

        data = ResMan::Instance()->readFile(fileName, &len);


        if (type.compare("WSA") == 0)
        {
            Wsafile* wsafile(new Wsafile(data, len));
        	
            animation = wsafile->getAnimation(0,wsafile->getNumFrames() - 1, palette, false);
            float frameRate = 1.0;
            node.lookupValue("frame_rate", frameRate);
            animation->setFrameRate(frameRate);

            delete wsafile;

        }
        
        if (type.compare("SHP") == 0)
        {
            int startIndex, endIndex;
            float frameRate = 1.0;
            node.lookupValue("start_index", startIndex);
            node.lookupValue("end_index", endIndex);

            Shpfile * shpfile = new Shpfile(data, len, palette);
            animation = shpfile->getAnimation(startIndex,endIndex,true);

            node.lookupValue("frame_rate", frameRate);
       		animation->setFrameRate(frameRate);

       		delete shpfile;
    
        }
    
    }
    catch(ParseException& ex)
    {
        LOG_FATAL("DataCache", "Setting not found %d: %s", 
            ex.getLine(), ex.getError());

        exit(EXIT_FAILURE);
    }


    return animation;   
}

Mix_Chunk* DataCache::concat2Chunks(std::string ID1, std::string ID2)
{
//	return concat2Chunks(soundChunk[ID1], soundChunk[ID2]);
    return 0;
}

DataCache::~DataCache() {

}
