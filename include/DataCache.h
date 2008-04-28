#ifndef DUNE_DATACACHE_H
#define DUNE_DATACACHE_H

#include "Application.h"
#include "Gfx.h"
#include "Settings.h"
#include "ResMan.h"
#include "singleton.h"
#include "SDL_mixer.h"

#include "pakfile/Cpsfile.h"
#include "pakfile/Icnfile.h"
#include "pakfile/Palette.h"
#include "pakfile/Shpfile.h"
#include "pakfile/Wsafile.h"
#include "pakfile/Stringfile.h"
#include "houses.h"

#include <map>
#include <vector>

#define NUM_MAPCHOICEPIECES	28
#define NUM_MAPCHOICEARROWS	9
class Animation;

// ObjPics
typedef enum {
	ObjPic_Tank_Base,
	ObjPic_Tank_Gun,
	ObjPic_Siegetank_Base,
	ObjPic_Siegetank_Gun,
	ObjPic_Devastator_Base,
	ObjPic_Devastator_Gun,
	ObjPic_Sonictank_Gun,
	ObjPic_Launcher_Gun,
	ObjPic_Quad,
	ObjPic_Trike,
	ObjPic_Harvester,
	ObjPic_Harvester_Sand,
	ObjPic_MCV,
	ObjPic_Carryall,
	ObjPic_Frigate,
	ObjPic_Ornithopter,
	ObjPic_Trooper,
	ObjPic_Infantry,
	ObjPic_Saboteur,
	ObjPic_Sandworm,
	ObjPic_ConstructionYard,
	ObjPic_Windtrap,
	ObjPic_Refinery,
	ObjPic_Barracks,
	ObjPic_WOR,
	ObjPic_Radar,
	ObjPic_LightFactory,
	ObjPic_Silo,
	ObjPic_HeavyFactory,
	ObjPic_HighTechFactory,
	ObjPic_IX,
	ObjPic_Palace,
	ObjPic_RepairYard,
	ObjPic_Starport,
	ObjPic_GunTurret,
	ObjPic_RocketTurret,
	ObjPic_Wall,
	ObjPic_Bullet_SmallRocket,
	ObjPic_Bullet_MediumRocket,
	ObjPic_Bullet_LargeRocket,
	ObjPic_Bullet_Small,
	ObjPic_Bullet_Medium,
	ObjPic_Bullet_Sonic,
	ObjPic_Hit_Gas,
	ObjPic_Hit_Shell,
	ObjPic_ExplosionSmall,
	ObjPic_ExplosionMedium1,
	ObjPic_ExplosionMedium2,
	ObjPic_ExplosionLarge1,
	ObjPic_ExplosionLarge2,
	ObjPic_ExplosionSmallUnit,
	ObjPic_DeadInfantry,
	ObjPic_Smoke,
	ObjPic_SandwormShimmerMask,
	ObjPic_Terrain,
	ObjPic_RockDamage,
	ObjPic_SandDamage,
	ObjPic_Terrain_Hidden,
	NUM_OBJPICS
} ObjPic_enum;

#define GROUNDUNIT_ROW(i) (i+2)|TILE_NORMAL,(i+1)|TILE_NORMAL,i|TILE_NORMAL,(i+1)|TILE_FLIPV,(i+2)|TILE_FLIPV,(i+3)|TILE_FLIPV, (i+4)|TILE_NORMAL,(i+3)|TILE_NORMAL
#define AIRUNIT_ROW(i) (i+2)|TILE_NORMAL,(i+1)|TILE_NORMAL,i|TILE_NORMAL,(i+1)|TILE_FLIPV,(i+2)|TILE_FLIPV,(i+1)|TILE_ROTATE, i|TILE_FLIPH,(i+1)|TILE_FLIPH
#define ORNITHOPTER_ROW(i) (i+6)|TILE_NORMAL,(i+3)|TILE_NORMAL,i|TILE_NORMAL,(i+3)|TILE_FLIPV,(i+6)|TILE_FLIPV,(i+3)|TILE_ROTATE, i|TILE_FLIPH,(i+3)|TILE_FLIPH
#define INFANTRY_ROW(i) (i+3)|TILE_NORMAL,i|TILE_NORMAL,(i+3)|TILE_FLIPV,(i+6)|TILE_NORMAL
#define HARVESTERSAND_ROW(i) (i+6)|TILE_NORMAL,(i+3)|TILE_NORMAL,i|TILE_NORMAL,(i+3)|TILE_FLIPV,(i+6)|TILE_FLIPV,(i+9)|TILE_FLIPV,(i+12)|TILE_NORMAL,(i+9)|TILE_NORMAL
#define ROCKET_ROW(i)	(i+4)|TILE_NORMAL,(i+3)|TILE_NORMAL,(i+2)|TILE_NORMAL,(i+1)|TILE_NORMAL,i|TILE_NORMAL,(i+1)|TILE_FLIPV,(i+2)|TILE_FLIPV,(i+3)|TILE_FLIPV, \
						(i+4)|TILE_FLIPV,(i+3)|TILE_ROTATE,(i+2)|TILE_ROTATE, (i+1)|TILE_ROTATE,i|TILE_FLIPH,(i+1)|TILE_FLIPH,(i+2)|TILE_FLIPH,(i+3)|TILE_FLIPH


// SmallDetailPics
typedef enum {
	Picture_Barracks,
	Picture_ConstructionYard,
	Picture_Carryall,
	Picture_Devastator,
	Picture_Deviator,
	Picture_DeathHand,
	Picture_Fremen,
	Picture_GunTurret,
	Picture_Harvester,
	Picture_HeavyFactory,
	Picture_HighTechFactory,
	Picture_Infantry,
	Picture_IX,
	Picture_Launcher,
	Picture_LightFactory,
	Picture_MCV,
	Picture_Ornithopter,
	Picture_Palace,
	Picture_Quad,
	Picture_Radar,
	Picture_Raider,
	Picture_Refinery,
	Picture_RepairYard,
	Picture_RocketTurret,
	Picture_Saboteur,
	Picture_Sardaukar,
	Picture_SiegeTank,
	Picture_Silo,
	Picture_Slab1,
	Picture_Slab4,
	Picture_SonicTank,
	Picture_StarPort,
	Picture_Tank,
	Picture_Trike,
	Picture_Trooper,
	Picture_Wall,
	Picture_WindTrap,
	Picture_WOR,
	NUM_SMALLDETAILPICS
} SmallDetailPics_enum;

// UI Graphics
typedef enum {
	UI_RadarAnimation,
	UI_CursorShape,
	UI_CreditsDigits,
	UI_GameBar,
	UI_Indicator,
	UI_InvalidPlace,
	UI_ValidPlace,
	UI_MenuBackground,
	UI_Background,
	UI_SelectionBox,
	UI_TopBar,
	UI_ButtonUp,
	UI_ButtonUp_Pressed,
	UI_ButtonDown,
	UI_ButtonDown_Pressed,
	UI_MessageBox,
	UI_Mentat,
	UI_Mentat_Pressed,
	UI_Options,
	UI_Options_Pressed,
	UI_Upgrade,
	UI_Upgrade_Pressed,
	UI_Repair,
	UI_Repair_Pressed,
	UI_Difficulty,
	UI_HeraldAtre_Coloured,
	UI_HeraldHark_Coloured,
	UI_HeraldOrd_Coloured,
	UI_Dif_Easy,
	UI_Dif_Hard,
	UI_Dif_Medium,
	UI_Minus,
	UI_Minus_Pressed,
	UI_Plus,
	UI_Plus_Pressed,
	UI_HouseSelect,
	UI_MissionSelect,
	UI_OptionsMenu,
	UI_LoadSaveWindow,
	UI_GameMenu,
	UI_HouseChoiceBackground,
	UI_MentatBackground,
	UI_MentatYes,
	UI_MentatYes_Pressed,
	UI_MentatNo,
	UI_MentatNo_Pressed,
	UI_MentatExit,
	UI_MentatExit_Pressed,	
	UI_MentatProcced,
	UI_MentatProcced_Pressed,
	UI_MentatRepeat,
	UI_MentatRepeat_Pressed,
	UI_PlanetBackground,
	UI_MenuButtonBorder,
	UI_DuneLegacy,
	UI_MapChoiceScreen,
	UI_MapChoiceMapOnly,
	UI_MapChoiceMap,
	UI_MapChoiceClickMap,
	NUM_UIGRAPHICS
} GuiPic_enum;

//Animation
typedef enum {
	Anim_AtreidesEyes,
	Anim_AtreidesMouth,
	Anim_AtreidesShoulder,
	Anim_AtreidesBook,
	Anim_HarkonnenEyes,
	Anim_HarkonnenMouth,
	Anim_HarkonnenShoulder,
	Anim_OrdosEyes,
	Anim_OrdosMouth,
	Anim_OrdosShoulder,
	Anim_OrdosRing,
	Anim_BeneGesseritEyes,
	Anim_BeneGesseritMouth,
	Anim_BeneGesseritShoulder,
	Anim_AtreidesPlanet,
	Anim_HarkonnenPlanet,
	Anim_OrdosPlanet,
	Anim_Win1,
	Anim_Win2,
	Anim_Lose1,
	Anim_Lose2,
	Anim_Barracks,
	Anim_Carryall,
	Anim_ConstructionYard,
	Anim_Fremen,
	Anim_DeathHand,
	Anim_Devastator,
	Anim_Harvester,
	Anim_Radar,
	Anim_HighTechFactory,
	Anim_SiegeTank,
	Anim_HeavyFactory,
	Anim_Trooper,
	Anim_Infantry,
	Anim_IX,
	Anim_LightFactory,
	Anim_Tank,
	Anim_MCV,
	Anim_Deviator,
	Anim_Ornithopter,
	Anim_Raider,
	Anim_Palace,
	Anim_Quad,
	Anim_Refinery,
	Anim_RepairYard,
	Anim_Launcher,
	Anim_RocketTurret,
	Anim_Saboteur,
	Anim_Slab1,
	Anim_SonicTank,
	Anim_StarPort,
	Anim_Silo,
	Anim_Trike,
	Anim_GunTurret,
	Anim_Wall,
	Anim_WindTrap,
	Anim_WOR,
	Anim_Sandworm,
	Anim_Sardaukar,
	Anim_Frigate,
	Anim_Slab4,
	NUM_ANIMATION
} Animation_enum;

// Voice
typedef enum {
	HarvesterDeployed,
	ConstructionComplete,
	VehicleRepaired,
	FrigateHasArrived,
	YourMissionIsComplete,
	YouHaveFailedYourMission,
	RadarActivated,
	RadarDeactivated,
	NUM_VOICE
} Voice_enum;

// Sound
typedef enum {
	YesSir,
	Reporting,
	Acknowledged,
	Affirmative,
	MovingOut,
	InfantryOut,
	SomethingUnderTheSand,
	HouseAtreides,
	HouseOrdos,
	HouseHarkonnen,
	PlaceStructure,
	ButtonClick,
//	InvalidAction,
	CreditsTick,
	RadarNoise,
	Sound_ExplosionGas,
	Sound_ExplosionTiny,
	Sound_ExplosionSmall,
	Sound_ExplosionMedium,
	Sound_ExplosionLarge,
	Sound_ExplosionStructure,
	Sound_WormAttack,
	Sound_Gun,
	Sound_Rocket,
	Sound_Bloom,
	Sound_Scream1,
	Sound_Scream2,
	Sound_Scream3,
	Sound_Scream4,
	Sound_Scream5,	
	Sound_MachineGun,
//	Sound_Sonic,
	Sound_RocketSmall,
	Intro_3Houses,
	Intro_AndNow,
	Intro_Battle,
	Intro_Begins,
	Intro_Blaster,
	Intro_TheBuilding,
	Intro_Blowup_1,
	Intro_Blowup_2,
	Intro_Brakes_2p,
	Intro_Clank,
	Intro_Click,
	Intro_ControlsDune,
	Intro_ControlsTheSpice,
	Intro_ForControl,
	Intro_Controls,
	Intro_Dune,
	Intro_OfADynasty,
	Intro_ToEachOfTheHouses,
	Intro_AndNo,
	Intro_WillControlDune,
	Intro_TheHouse,
	Intro_TheMostSpice,
	Intro_TheEmpire,	
	Intro_ThereAreNoSetTerritories,
	Intro_ThatProduces,
	Intro_RulesOfEngagement,
	Intro_Territories,
	Intro_AndThe,
//	Intro_Filler,
	Intro_Flesh,
	Intro_Glass_6,
	Intro_Glass,
	Intro_GunShot,
	Intro_EvilHarkonnen,	
	Intro_Home,
	Intro_OnlyOneHouse,
	Intro_TheInsideous,
	Intro_TheEmperor,
	Intro_KnownAsDune,
	Intro_Worm,
	Intro_Melange,
	Intro_Missile_8,
	Intro_TheNobleAtreides,
	Intro_Now,
	Intro_OfDune,
	Intro_Ordos,
	Intro_ThePlanetArrakis,
	Intro_WillPrevail,
	Intro_HasProposedAChallenge,
	Intro_LandOfSand,
	Intro_OfTheSpice,
	Intro_TheSpice,
	Intro_VastArmies,
	Intro_WhoEver,
	Intro_Wind_2bp,
	Intro_Your,
	NUM_SOUNDCHUNK
} Sound_enum;

typedef std::map <unsigned, ImagePtr> images;
typedef std::vector <images*> remapped_images; //One for each house

class DataCache : public Singleton<DataCache> 
{
    friend class Singleton<DataCache>;
    
    protected:
        DataCache();
        ~DataCache();

    public:
        void addObjPic(ObjPic_enum ID, Image * tmp, HOUSETYPE house = HOUSE_HARKONNEN);
        void addGuiPic(GuiPic_enum ID, Image * tmp, HOUSETYPE house = HOUSE_HARKONNEN);
		void addAnimation(Animation_enum ID, std::string filename, double frameRate = 0);
		void addAnimation(Animation_enum ID, Animation* animation, double frameRate = 0);
		void addSoundChunk(Sound_enum ID, Mix_Chunk* tmp); 
        ImagePtr	getObjPic(ObjPic_enum ID, HOUSETYPE house = HOUSE_HARKONNEN);
        ImagePtr	getGuiPic(GuiPic_enum ID, HOUSETYPE house = HOUSE_HARKONNEN);
		Animation*		getAnimation(Animation_enum id);
		Mix_Chunk* getSoundChunk(Sound_enum ID);
		Mix_Chunk* concat2Chunks(Sound_enum ID1, Sound_enum ID2);
		std::string	getBriefingText(uint16_t mission, uint16_t textType, HOUSETYPE house);

//		std::string	getBriefingText(ObjPic_enum mission, ObjPic_enum texttype, ObjPic_enum house);
//		std::string	getBriefingText(ObjPic_enum i);



    private:
        remapped_images m_objImg;
        remapped_images m_guiImg;
		
		Mix_Chunk* getChunkFromFile(std::string fileName);
		Mix_Chunk* concat2Chunks(Mix_Chunk* sound1, Mix_Chunk* sound2);
		Mix_Chunk* concat3Chunks(Mix_Chunk* sound1, Mix_Chunk* sound2, Mix_Chunk* sound3);
		Mix_Chunk* createEmptyChunk();

		Animation*		Anim[NUM_ANIMATION];
		Stringfile*	BriefingStrings[3];
		Mix_Chunk*		soundChunk[NUM_SOUNDCHUNK];

};

#endif // DUNE_DATACACHE_H
