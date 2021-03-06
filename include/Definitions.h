#ifndef DUNE_DEFINITIONS_H
#define DUNE_DEFINITIONS_H
//TODO: Sort it somehow.

#include <cstdint>
#include <memory>
#include <map>
#include <SDL.h>
#include <eastwood/SDL/Mixer/Sound.h>

using std::swap;

class Image;
class ObjectClass;

//! Smart pointer around Image class instance
typedef std::shared_ptr<Image> ImagePtr;

//! Smart pointer around Image class const instance
typedef std::shared_ptr<const Image> ConstImagePtr;

//! The Image class is a wrapper around SDL_Surface
/*!
    It allows using SDL_Surfaces with smart pointers.
    Use ConstImagePtr and ImagePtr where possible (former is more
    favorable), it avoids memory leaks...
        
*/

typedef std::shared_ptr<eastwood::SDL::Mixer::Sound> SoundPtr;

typedef std::shared_ptr<ObjectClass> ObjectPtr;
typedef std::pair<uint32_t, ObjectPtr> ObjectPair;
typedef std::map<uint32_t, ObjectPtr> ObjectMap;
typedef std::map<uint32_t, ObjectMap> ObjectTypeMap;

//! \enum MUSICTYPE
/*! Types of music available in the game*/
typedef enum { MUSIC_ATTACK, /*!<Played when at least one of player's units was hit. */
			   MUSIC_LOSE,   /*!<Failure screen background music. */
			   MUSIC_PEACE,  /*!<Played most of the time when the enemy is not attacking. */
			   MUSIC_WIN,    /*!<Victory screen background music.. */
			   MUSIC_INTRO,  /*!<Background music for intro. */
			   MUSIC_RANDOM  /*!<Player used key combination to change current music. */
			 } MUSICTYPE;


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
	ObjPic_WindTrap,
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
	ObjPic_StarPort,
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
	UI_MouseCursor,
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
    UI_TopBorder,
    UI_BottomBorder,
    UI_LeftBorder,
    UI_RightBorder,
    UI_Corner1NW,
    UI_Corner1NE,
    UI_Corner1SW,
    UI_Corner1SE,
    UI_Corner2NW,
    UI_Corner2NE,
    UI_Corner2SW,
    UI_Corner2SE,
    UI_Corner3NW,
    UI_Corner3NE,
    UI_Corner3SW,
    UI_Corner3SE,
    UI_BlankFiller,
    UI_Mentat_HeraldHarkonnen,
    UI_Mentat_HeraldAtreides,
    UI_Mentat_HeraldOrdos,
	NUM_UIGRAPHICS
} GuiPic_enum;

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
	Intro_HasArrived,	
	Intro_BattleForDune,
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
	Intro_ThereAreNoSet,
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


typedef enum {
	Terrain_Dunes,
	Terrain_Hidden,
	Terrain_Mountain,
	Terrain_Rock,
	Terrain_RockDamage,
	Terrain_Sand,
	Terrain_SandDamage,
	Terrain_Spice,
	Terrain_ThickSpice	
} TerrainID_enum;

//definitions for positions in the spice, rock and wall images


typedef enum
{
	Terrain_HiddenIsland,	
	Terrain_HiddenUp,
	Terrain_HiddenRight,
	Terrain_HiddenUpRight,
	Terrain_HiddenDown,
	Terrain_HiddenUpDown,
	Terrain_HiddenDownRight,
	Terrain_HiddenNotLeft,
	Terrain_HiddenLeft,
	Terrain_HiddenUpLeft,
	Terrain_HiddenLeftRight,
	Terrain_HiddenNotDown,
	Terrain_HiddenDownLeft,
	Terrain_HiddenNotRight,
	Terrain_HiddenNotUp,
	Terrain_HiddenFull

	/*
	Terrain_HiddenFull,
	Terrain_HiddenIsland,
	Terrain_HiddenLeftRight,
	Terrain_HiddenLeft,
	Terrain_HiddenRight,
	Terrain_HiddenUp,
	Terrain_HiddenDown,
	Terrain_HiddenUpDown,
	Terrain_HiddenNotLeft,
	Terrain_HiddenNotRight,
	Terrain_HiddenNotUp,
	Terrain_HiddenNotDown,
	Terrain_HiddenDownRight,
	Terrain_HiddenUpLeft,
	Terrain_HiddenDownLeft,
	Terrain_HiddenUpRight
	*/
} HIDDENTYPE;

/*

typedef enum
{
	Terrain_MountainFull,
	Terrain_MountainIsland,
	Terrain_MountainLeftRight,
	Terrain_MountainLeft,
	Terrain_MountainRight,
	Terrain_MountainUp,
	Terrain_MountainDown,
	Terrain_MountainUpDown,
	Terrain_MountainNotLeft,
	Terrain_MountainNotRight,
	Terrain_MountainNotUp,
	Terrain_MountainNotDown,
	Terrain_MountainDownRight,
	Terrain_MountainUpLeft,
	Terrain_MountainDownLeft,
	Terrain_MountainUpRight
} MOUNTAINTYPE;


typedef enum
{
	Terrain_DunesFull,
	Terrain_DunesIsland,
	Terrain_DunesLeftRight,
	Terrain_DunesLeft,
	Terrain_DunesRight,
	Terrain_DunesUp,
	Terrain_DunesDown,
	Terrain_DunesUpDown,
	Terrain_DunesNotLeft,
	Terrain_DunesNotRight,
	Terrain_DunesNotUp,
	Terrain_DunesNotDown,
	Terrain_DunesDownRight,
	Terrain_DunesUpLeft,
	Terrain_DunesDownLeft,
	Terrain_DunesUpRight
} DUNESTYPE;


typedef enum
{
	Terrain_a1,
	Terrain_a2,
	Terrain_a3
} SANDTYPE;


*/
typedef enum
{
	Terrain_sd1,
	Terrain_sd2,
	Terrain_sd3
} SANDDAMAGETYPE;

/*

typedef enum
{
	Terrain_s1,
	Terrain_s10,
	Terrain_s11,
	Terrain_s12,
	Terrain_s13,
	Terrain_s14,
	Terrain_s15,
	Terrain_s16,
	Terrain_s2,
	Terrain_s3,
	Terrain_s4,
	Terrain_s5,
	Terrain_s6,
	Terrain_s7,
	Terrain_s8,
	Terrain_s9
} SPICETYPE;

typedef enum
{
	Terrain_ThickSpiceFull,
	Terrain_ThickSpiceIsland,
	Terrain_ThickSpiceLeftRight,
	Terrain_ThickSpiceLeft,
	Terrain_ThickSpiceRight,
	Terrain_ThickSpiceUp,
	Terrain_ThickSpiceDown,
	Terrain_ThickSpiceUpDown,
	Terrain_ThickSpiceNotLeft,
	Terrain_ThickSpiceNotRight,
	Terrain_ThickSpiceNotUp,
	Terrain_ThickSpiceNotDown,
	Terrain_ThickSpiceDownRight,
	Terrain_ThickSpiceUpLeft,
	Terrain_ThickSpiceDownLeft,
	Terrain_ThickSpiceUpRight
} THICKSPICETYPE;

*/

typedef enum
{
	Terrain_td1,
	Terrain_td2
} ROCKDAMAGETYPE;

/*
typedef enum
{
	Terrain_t1,
	Terrain_t10,
	Terrain_t11,
	Terrain_t12,
	Terrain_t13,
	Terrain_t14,
	Terrain_t15,
	Terrain_t16,
	Terrain_t2,
	Terrain_t3,
	Terrain_t4,
	Terrain_t5,
	Terrain_t6,
	Terrain_t7,
	Terrain_t8,
	Terrain_t9,

	Terrain_tc1x1,
	Terrain_tc2x2TopLeft,
	Terrain_tc2x2TopRight,
	Terrain_tc2x2BottomLeft,
	Terrain_tc2x2BottomRight,
	Terrain_tc3x3TopLeft,
	Terrain_tc3x3TopCentre,
	Terrain_tc3x3TopRight,
	Terrain_tc3x3MiddleLeft,
	Terrain_tc3x3MiddleCentre,
	Terrain_tc3x3MiddleRight,
	Terrain_tc4x4BottomLeft,
	Terrain_tc4x4BottomCentre,
	Terrain_tc4x4BottomRight
} ROCKTYPE;

*/
typedef enum
{
	Structure_w1,
	Structure_w10,
	Structure_w11,
	Structure_w2,
	Structure_w3,
	Structure_w4,
	Structure_w5,
	Structure_w6,
	Structure_w7,
	Structure_w8,
	Structure_w9
} WALLTYPE;

typedef enum
{
	Terrain_SlabHalfDestroyed,
	Terrain_SlabDestroyed,
	Terrain_Slab,
	Terrain_a1,
	Terrain_t10,
	Terrain_t14,
	Terrain_t13,
	Terrain_t9,
	Terrain_t15,
	Terrain_t16,
	Terrain_t6,
	Terrain_t2,
	Terrain_t12,
	Terrain_t7,
	Terrain_t11,
	Terrain_t5,
	Terrain_t8,
	Terrain_t3,
	Terrain_t4,
	Terrain_t1,
	Terrain_DunesIsland,
	Terrain_DunesUp,
	Terrain_DunesRight,
	Terrain_DunesUpRight,
	Terrain_DunesDown,
	Terrain_DunesUpDown,
	Terrain_DunesDownRight,
	Terrain_DunesNotLeft,
	Terrain_DunesLeft,
	Terrain_DunesUpLeft,	
	Terrain_DunesLeftRight,
	Terrain_DunesNotDown,
	Terrain_DunesDownLeft,
	Terrain_DunesNotRight,
	Terrain_DunesNotUp,
	Terrain_DunesFull,	
	Terrain_MountainIsland,
	Terrain_MountainUp,
	Terrain_MountainRight,
	Terrain_MountainUpRight,
	Terrain_MountainDown,
	Terrain_MountainUpDown,
	Terrain_MountainDownRight,
	Terrain_MountainNotLeft,
	Terrain_MountainLeft,
	Terrain_MountainUpLeft,
	Terrain_MountainLeftRight,
	Terrain_MountainNotDown,
	Terrain_MountainDownLeft,
	Terrain_MountainNotRight,
	Terrain_MountainNotUp,
	Terrain_MountainFull,	
	Terrain_s10,
	Terrain_s14,
	Terrain_s13,
	Terrain_s9,
	Terrain_s15,
	Terrain_s16,
	Terrain_s6,
	Terrain_s2,
	Terrain_s12,
	Terrain_s7,
	Terrain_s11,
	Terrain_s5,
	Terrain_s8,
	Terrain_s3,
	Terrain_s4,
	Terrain_s1,
	Terrain_ThickSpiceIsland,
	Terrain_ThickSpiceUp,
	Terrain_ThickSpiceRight,
	Terrain_ThickSpiceUpRight,
	Terrain_ThickSpiceDown,
	Terrain_ThickSpiceUpDown,
	Terrain_ThickSpiceDownRight,
	Terrain_ThickSpiceNotLeft,
	Terrain_ThickSpiceLeft,
	Terrain_ThickSpiceUpLeft,
	Terrain_ThickSpiceLeftRight,
	Terrain_ThickSpiceNotDown,
	Terrain_ThickSpiceDownLeft,
	Terrain_ThickSpiceNotRight,
	Terrain_ThickSpiceNotUp,
	Terrain_ThickSpiceFull,
	Terrain_a2,
	Terrain_a3
	
} TERRAINTYPE;

typedef enum {
	Unknown = 0,
	Bullet_DRocket = 1,
	Bullet_GasHit = 2,
	Bullet_Gun = 3,
	Bullet_Inf_Gun = 4,
	Bullet_LargeRocket = 5,
	Bullet_LargeShot = 6,
	Bullet_Rocket = 7,
	Bullet_RocketHit = 8,
	Bullet_Shell = 9,
	Bullet_ShellHit = 10,
	Bullet_SmallRocket = 11,
	Bullet_SmallRocketHeavy = 12,
	Bullet_SmallShot = 13,
	Bullet_Sonic = 14,
	Structure_BuildingTurret = 15,
	Structure_Barracks = 16,
	Structure_ConstructionYard = 17,
	Structure_ConstructionYardPlace = 18,
	Structure_GunTurret = 19,
	Structure_HeavyFactory = 20,
	Structure_HighTechFactory = 21,
	Structure_IX = 22,
	Structure_LightFactory = 23,
	Structure_Palace = 24,
	Structure_Radar = 25,
	Structure_Refinery = 26,
	Structure_RepairYard = 27,
	Structure_RocketTurret = 28,
	Structure_Silo = 29,
	Structure_Slab1 = 30,
	Structure_Slab4 = 31,
	Structure_StarPort = 32,
	Structure_Wall = 33,
	Structure_WindTrap = 34,
	Structure_WOR = 35,
	Unit_Carryall = 36,
	Unit_Devastator = 37,
	Unit_DevastatorBase = 38,
	Unit_Deviator = 39,
	Unit_Fremen = 40,
	Unit_Frigate = 41,
	Unit_Harvester = 42,
	Unit_HarvesterSand = 43,
	Unit_Infantry = 44,
	Unit_Launcher = 45,
	Unit_MCV = 46,
	Unit_Ornithopter = 47,
	Unit_Quad = 48,
	Unit_Saboteur = 49,
	Unit_Sandworm = 50,
	Unit_Sardaukar = 51,
	Unit_SiegeTank = 52,
	Unit_SiegeTankBase = 53,
	Unit_SonicTank = 54,
	Unit_Tank = 55,
	Unit_TankBase = 56,
	Unit_Trike = 57,
	Unit_Raider = 58,
	Unit_Trooper = 59	
} ObjectID_enum;

typedef enum {
	CT_EMUOPL = 1,	
	C_EMUOPL,
	CK_EMUOPL,
	CREAL_EMUOPL,
	CANAL_EMUOPL
} EMUOPL;
#endif // DUNE_DEFINITIONS_H
