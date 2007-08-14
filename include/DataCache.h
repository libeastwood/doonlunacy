#ifndef DUNE_DATACACHE_H
#define DUNE_DATACACHE_H

#include "Application.h"
#include "Gfx.h"
#include "Settings.h"
#include "ResMan.h"
#include "singleton.h"

#include "pakfile/Cpsfile.h"
#include "pakfile/Icnfile.h"
#include "pakfile/Palette.h"
#include "pakfile/Shpfile.h"
#include "pakfile/Wsafile.h"

#include "houses.h"

#include <map>
#include <vector>

typedef enum {
	ObjImg_Tank_Base,
	ObjImg_Tank_Gun,
	ObjImg_Siegetank_Base,
	ObjImg_Siegetank_Gun,
	ObjImg_Devastator_Base,
	ObjImg_Devastator_Gun,
	ObjImg_Sonictank_Gun,
	ObjImg_Launcher_Gun,
	ObjImg_Quad,
	ObjImg_Trike,
	ObjImg_Harvester,
	ObjImg_Harvester_Sand,
	ObjImg_MCV,
	ObjImg_Carryall,
	ObjImg_Frigate,
	ObjImg_Ornithopter,
	ObjImg_Trooper,
	ObjImg_Infantry,
	ObjImg_Saboteur,
	ObjImg_Sandworm,
	ObjImg_ConstructionYard,
	ObjImg_Windtrap,
	ObjImg_Refinery,
	ObjImg_Barracks,
	ObjImg_WOR,
	ObjImg_Radar,
	ObjImg_LightFactory,
	ObjImg_Silo,
	ObjImg_HeavyFactory,
	ObjImg_HighTechFactory,
	ObjImg_IX,
	ObjImg_Palace,
	ObjImg_RepairYard,
	ObjImg_Starport,
	ObjImg_GunTurret,
	ObjImg_RocketTurret,
	ObjImg_Wall,
	ObjImg_Bullet_SmallRocket,
	ObjImg_Bullet_MediumRocket,
	ObjImg_Bullet_LargeRocket,
	ObjImg_Bullet_Small,
	ObjImg_Bullet_Medium,
	ObjImg_Bullet_Sonic,
	ObjImg_Hit_Gas,
	ObjImg_Hit_Shell,
	ObjImg_ExplosionSmall,
	ObjImg_ExplosionMedium1,
	ObjImg_ExplosionMedium2,
	ObjImg_ExplosionLarge1,
	ObjImg_ExplosionLarge2,
	ObjImg_ExplosionSmallUnit,
	ObjImg_DeadInfantry,
	ObjImg_Smoke,
	ObjImg_SandwormShimmerMask,
	ObjImg_Terrain,
	ObjImg_RockDamage,
	ObjImg_SandDamage,
	ObjImg_Terrain_Hidden,
	NUM_OBJIMGS
} ObjImg_enum;

#define GROUNDUNIT_ROW(i) (i+2)|TILE_NORMAL,(i+1)|TILE_NORMAL,i|TILE_NORMAL,(i+1)|TILE_FLIPV,(i+2)|TILE_FLIPV,(i+3)|TILE_FLIPV, (i+4)|TILE_NORMAL,(i+3)|TILE_NORMAL
#define AIRUNIT_ROW(i) (i+2)|TILE_NORMAL,(i+1)|TILE_NORMAL,i|TILE_NORMAL,(i+1)|TILE_FLIPV,(i+2)|TILE_FLIPV,(i+1)|TILE_ROTATE, i|TILE_FLIPH,(i+1)|TILE_FLIPH
#define ORNITHOPTER_ROW(i) (i+6)|TILE_NORMAL,(i+3)|TILE_NORMAL,i|TILE_NORMAL,(i+3)|TILE_FLIPV,(i+6)|TILE_FLIPV,(i+3)|TILE_ROTATE, i|TILE_FLIPH,(i+3)|TILE_FLIPH
#define INFANTRY_ROW(i) (i+3)|TILE_NORMAL,i|TILE_NORMAL,(i+3)|TILE_FLIPV,(i+6)|TILE_NORMAL
#define HARVESTERSAND_ROW(i) (i+6)|TILE_NORMAL,(i+3)|TILE_NORMAL,i|TILE_NORMAL,(i+3)|TILE_FLIPV,(i+6)|TILE_FLIPV,(i+9)|TILE_FLIPV,(i+12)|TILE_NORMAL,(i+9)|TILE_NORMAL
#define ROCKET_ROW(i)	(i+4)|TILE_NORMAL,(i+3)|TILE_NORMAL,(i+2)|TILE_NORMAL,(i+1)|TILE_NORMAL,i|TILE_NORMAL,(i+1)|TILE_FLIPV,(i+2)|TILE_FLIPV,(i+3)|TILE_FLIPV, \
						(i+4)|TILE_FLIPV,(i+3)|TILE_ROTATE,(i+2)|TILE_ROTATE, (i+1)|TILE_ROTATE,i|TILE_FLIPH,(i+1)|TILE_FLIPH,(i+2)|TILE_FLIPH,(i+3)|TILE_FLIPH


typedef std::map <unsigned, ImagePtr> images;
typedef std::vector <images*> remapped_images; //One for each house

class DataCache : public Singleton<DataCache> 
{
    friend class Singleton<DataCache>;
    
    protected:
        DataCache();
        ~DataCache();

    public:
        void addObjImg(unsigned ID, SDL_Surface * tmp);
        ImagePtr getObjImg(unsigned ID, unsigned house = HOUSE_HARKONNEN);
    private:
        bool addObjImg(unsigned ID) { return false;};
        remapped_images m_objImg;
        remapped_images m_guiImg;
};

#endif // DUNE_DATACACHE_H
