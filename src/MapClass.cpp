#include "MapClass.h"

MapClass::MapClass(UPoint size)
{
    w = size.x;
    h = size.y;
    
    int i;
    for (i = 0; i < w*h; i++)
    {
        m_cells.push_back(new TerrainClass());
    }
}

MapClass::~MapClass()
{

}


