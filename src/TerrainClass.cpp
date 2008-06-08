#include "DataCache.h"
#include "Definitions.h"
#include "Gfx.h"
#include "TerrainClass.h"

TerrainClass::TerrainClass()
{
    m_tile = Terrain_a1;
    m_type = Terrain_Sand;
    m_img = DataCache::Instance()->getObjPic(ObjPic_Terrain);
}

TerrainClass::~TerrainClass()
{

}

void TerrainClass::draw(Image * dest, SPoint pos)
{
    Rect source(m_tile*BLOCKSIZE, 0, BLOCKSIZE, BLOCKSIZE);
    m_img->blitTo(dest, source, pos);
}

