#include "GameData.h"

#include "DataCache.h"
#include "Gfx.h"
#include "ResMan.h"

#include <eastwood/CpsFile.h>
#include <eastwood/IcnFile.h>
#include <eastwood/PalFile.h>
#include <eastwood/ShpFile.h>
#include <eastwood/VocFile.h>

GameData::GameData(std::string path)
{
    m_path = path;
    // set this to true to avoid it being freed
    m_persistent = true;
    m_freeCounter = 0;
}

GameData::~GameData()
{
}

bool GameData::freeIfUnique()
{
    if(!m_persistent){
	if(m_surface.unique())
	{
	    m_surface.reset();
	    m_freeCounter++;
	}
	if(!m_remappedImg.empty())
	{
	    std::map<HOUSETYPE, ImagePtr>::iterator iter;
	    for ( iter  = m_remappedImg.begin();
		    iter != m_remappedImg.end();
		    ++iter )
	    {
		if((*iter).second.unique()){
		    (*iter).second.reset();
		    m_freeCounter++;
		}
	    }
	}
	if(m_sound.unique())
	{
	    m_sound.reset();
	    m_freeCounter++;
	}

	if(m_freeCounter > 50)
	    LOG_WARNING("GameData", "%s has been freed over 50 times!", m_path.c_str());
	return true;
    }
    return false;
}

void GameData::drawImage()
{
    DataCache *cache = DataCache::Instance();

    try
    {
	size_t len;
	uint8_t *data;

	std::string variable;
	int value;

	SDL_Palette* palette;
	if(!(variable = cache->getPyObjectAttribute<std::string>(m_path, "palette")).empty())
	    palette = DataCache::Instance()->getPalette(variable);
	else
	    palette = DataCache::Instance()->getPalette("DUNE:IBM.PAL");

	if(!(variable = cache->getPyObjectAttribute<std::string>(m_path, "filename")).empty()) {
	    // TODO: autodetection would be nice..
	    std::string type;
	    if((type = cache->getPyObjectAttribute<std::string>(m_path, "type")).empty())
		type = variable.substr(variable.length()-3, 3);

	    data = ResMan::Instance()->readFile(variable, &len);

	    if (type == "CPS")
	    {
		CpsFile cpsfile(data, len, palette);
		m_surface.reset(new Image(cpsfile.getSurface()));
	    }

	    if (type == "SHP")
	    {
		std::vector<Uint32> tiles = cache->getPyObjectVector<Uint32>(m_path, "tiles");
		ShpFile shpfile(data, len, palette);
		if((value = cache->getPyObjectAttribute<int>(m_path, "index")) >= 0)
		    m_surface.reset(new Image(shpfile.getSurface(value)));
		else if(!tiles.empty())
		{
		    Uint32 tilesX = 0, tilesY = 0;
		    for(Uint32 j = 0; j < tiles.size(); j++){ 
			if(TILE_GETINDEX(tiles[j]) >= (Uint32)shpfile.getNumFiles()) {
			    LOG_ERROR("GameData","ShpFile::getSurfaceArray(): There exist only %d files in this *.shp.",shpfile.getNumFiles());
			    exit(EXIT_FAILURE);
			}								
		    }
		    if(!tilesX)
			tilesX = tiles.size();
		    else if(tilesX != tiles.size())
			LOG_FATAL("GameData:", "Tile row size %d is of different size than %d for %s!",
				tiles.size(), tilesX, variable.c_str());
		    tilesY++;
		    Uint32 *tilesArray = new Uint32[tiles.size()];
		    std::copy(tiles.begin(), tiles.end(), tilesArray);
		    m_surface.reset(new Image(shpfile.getSurfaceArray(tilesX, tilesY, tilesArray)));
		    delete [] tilesArray;
		}
		else
		{
		    LOG_FATAL("GameData", "%s: No index or tiles specified for %s!", m_path.c_str(), variable.c_str());
		    exit(EXIT_FAILURE);
		}
	    }
	    if (type == "ICN") {
		std::string mapName;
		if(!(mapName = cache->getPyObjectAttribute<std::string>(m_path, "filename")).empty()) {
		    size_t mapLen;
		    uint8_t *mapData = ResMan::Instance()->readFile(mapName, &mapLen);

		    IcnFile icnfile(data, len, mapData, mapLen, palette);
    		    if((value = cache->getPyObjectAttribute<int>(m_path, "index")))
			m_surface.reset(new Image(icnfile.getSurface(value)));
		    else if(!cache->nonePyObject(m_path, "row"))
		    {
			UPoint row = cache->getPyObjectAttribute<UPoint>(m_path, "row");
			m_surface.reset(new Image(icnfile.getSurfaceRow(row.x, row.y)));
		    }
		    else if(!cache->nonePyObject(m_path, "mapindex"))
		    {
			int tilesN = cache->getPyObjectAttribute<int>(m_path, "num");
			UPoint tilePos = cache->getPyObjectAttribute<UPoint>(m_path, "tilepos");
			m_surface.reset(new Image(icnfile.getSurfaceArray(value, tilePos.x, tilePos.y, tilesN)));
		    }
		    else
		    {
			LOG_FATAL("GameData", "no index, mapindex or row specified for %s!", variable.c_str());
			exit(EXIT_FAILURE);
		    }
		}
		else
		{
		    LOG_FATAL("GameData", "No map specified for %s!", variable.c_str());
		    exit(EXIT_FAILURE);
		}
	    }
	}
	else if((variable = cache->getPyObjectType(m_path, "gcobject", 0)) != "NoneType")
	{
	    Uint32 colorkey = 0;
	    ImagePtr gcObj = DataCache::Instance()->getGameData(variable)->getImage();
	    if(!cache->nonePyObject(m_path, "crop")) {
		Rect crop = cache->getPyObjectAttribute<Rect>(m_path, "crop");
		m_surface.reset(gcObj->getPictureCrop(crop));
	    } else
		m_surface = gcObj->getCopy();

	    if(!cache->nonePyObject(m_path, "colorkey"))
	    {
		colorkey = cache->getPyObjectAttribute<int>(m_path, "colorkey");
		m_surface->setColorKey(colorkey);
	    }

	    if(!cache->nonePyObject(m_path, "putpixel"))
	    {
		std::vector<UPoint> putPixels = cache->getPyObjectVector<UPoint>(m_path, "putpixel");
	    	for (std::vector<UPoint>::const_iterator iter = putPixels.begin(); iter != putPixels.end(); iter++)
		    m_surface->putPixel(*iter, colorkey);
	    }

	    if(!cache->nonePyObject(m_path, "drawvline"))
	    {
		std::vector<Rect> drawVLines = cache->getPyObjectVector<Rect>(m_path, "drawvline");
	    	for (std::vector<Rect>::const_iterator iter = drawVLines.begin(); iter != drawVLines.end(); iter++)
		    m_surface->drawVLine((*iter).x, (*iter).y, (*iter).w, (*iter).h);
	    }

	    if(!cache->nonePyObject(m_path, "drawhline"))
	    {
		std::vector<Rect> drawHLines = cache->getPyObjectVector<Rect>(m_path, "drawhline");
	    	for (std::vector<Rect>::const_iterator iter = drawHLines.begin(); iter != drawHLines.end(); iter++)
		    m_surface->drawHLine((*iter).x, (*iter).y, (*iter).w, (*iter).h);
	    }

	    if(!cache->nonePyObject(m_path, "fillrect"))
	    {
		std::vector<Rect> fillRects = cache->getPyObjectVector<Rect>(m_path, "fillrect");
	    	for (std::vector<Rect>::const_iterator iter = fillRects.begin(); iter != fillRects.end(); iter++)
		    m_surface->fillRect(colorkey, *iter);
	    }
	}
	m_persistent = cache->getPyObjectAttribute<bool>(m_path, "persistent");
    }
    catch(python::error_already_set const &)
    {
	LOG_FATAL("GameData", "Error loading data: %s", m_path.c_str());
	PyErr_Print();
	exit(EXIT_FAILURE);
    }
}

ImagePtr GameData::getImage()
{
    if(!m_surface)
    {
	drawImage();
    }
    return m_surface;
}

ImagePtr GameData::getImage(HOUSETYPE house)
{
    std::map<HOUSETYPE, ImagePtr>::iterator iter = m_remappedImg.find(house);
    if (iter != m_remappedImg.end())
    { 
	return m_remappedImg.find(house)->second;
    }
    else
    {
	ImagePtr copy = getImage()->getRecoloredByHouse(house);
	m_remappedImg.insert(std::pair<HOUSETYPE, ImagePtr>(house, copy));
	return copy;
    }
}

SoundPtr GameData::getSound()
{
    if(!m_sound)
	m_sound.reset(DataCache::Instance()->getSoundChunk(m_path));

    return m_sound;
}

