#include "GameData.h"

#include "DataCache.h"
#include "Gfx.h"
#include "PythonObjects.h"
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
    try {
	size_t len;
	uint8_t *data;
	std::string variable;
	int value;
	UPoint pos;
	SDL_Palette* palette;
	python::object pyObject = DataCache::Instance()->getPyObject(m_path);

	if(getPyObject(pyObject.attr("palette"), &variable))
	    palette = DataCache::Instance()->getPalette(variable);
	else
	    LOG_ERROR("GameData", "No palette for %s!", m_path.c_str());

	if(getPyObjectType(pyObject, 1) == "GameDataConst") {
	    if(!getPyObject(pyObject.attr("filename"), &variable)) {
		LOG_ERROR("GameData", "%s: 'filename' variable missing!", variable.c_str());
		exit(EXIT_FAILURE);
	    }

	    // TODO: autodetection would be nice..
	    std::string type;
	    if(!getPyObject(pyObject.attr("type"), &type))
		type = variable.substr(variable.length()-3, 3);

	    data = ResMan::Instance()->readFile(variable, &len);

	    if (type == "CPS") {
		CpsFile cpsfile(data, len, palette);
		m_surface.reset(new Image(cpsfile.getSurface()));
	    }

	    if (type == "SHP") {
		std::vector<Uint32> tiles = getPyObjectVector<Uint32>(pyObject.attr("tiles"));
		ShpFile shpfile(data, len, palette);
		if(getPyObject<int>(pyObject.attr("index"), &value))
		    m_surface.reset(new Image(shpfile.getSurface(value)));
		else if(!tiles.empty()) {
		    Uint32 tilesX = 0, tilesY = 0;
		    for(Uint32 j = 0; j < tiles.size(); j++) { 
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
		else {
		    LOG_FATAL("GameData", "%s: No index or tiles specified for %s!", m_path.c_str(), variable.c_str());
		    exit(EXIT_FAILURE);
		}
	    }
	    if (type == "ICN") {
		std::string mapName;
		if(getPyObject<std::string>(pyObject.attr("map"), &mapName)) {
		    size_t mapLen;
		    uint8_t *mapData = ResMan::Instance()->readFile(mapName, &mapLen);

		    IcnFile icnfile(data, len, mapData, mapLen, palette);
    		    if(getPyObject<int>(pyObject.attr("index"), &value))
			m_surface.reset(new Image(icnfile.getSurface(value)));
		    else if(getPyObject<UPoint>(pyObject.attr("row"), &pos))
			m_surface.reset(new Image(icnfile.getSurfaceRow(pos.x, pos.y)));
		    else if(getPyObject<int>(pyObject.attr("mapindex"), &value)) {
			int tilesN = python::extract<int>(pyObject.attr("num"));
			UPoint tilePos = python::extract<UPoint>(pyObject.attr("tilepos"));
			m_surface.reset(new Image(icnfile.getSurfaceArray(value, tilePos.x, tilePos.y, tilesN)));
		    }
		    else {
			LOG_FATAL("GameData", "no index, mapindex or row specified for %s!", variable.c_str());
			exit(EXIT_FAILURE);
		    }
		}
		else {
		    LOG_FATAL("GameData", "No map specified for %s!", variable.c_str());
		    exit(EXIT_FAILURE);
		}
	    }
	}
	else if(getPyObjectType(pyObject, 1) == "GameDataMod") {
	    Uint32 colorkey = 0;
	    Rect crop;
	    ImagePtr gameData;
	    if((variable = getPyObjectType(pyObject.attr("gamedata"), 0)) != "NoneType")
		gameData = DataCache::Instance()->getGameData(variable)->getImage();
	    else {
		LOG_ERROR("GameMan", "%s: gamedata variable missing!", variable.c_str());
		exit(EXIT_FAILURE);
	    }
	    if(getPyObject(pyObject.attr("crop"), &crop))
		m_surface.reset(gameData->getPictureCrop(crop));
	    else
		m_surface = gameData->getCopy();

	    if(getPyObject(pyObject.attr("colorkey"), &colorkey))
		m_surface->setColorKey(colorkey);

	    if(!nonePyObject(pyObject.attr("putpixel"))) {
		std::vector<UPoint> putPixels = getPyObjectVector<UPoint>(pyObject.attr("putpixel"));
	    	for (std::vector<UPoint>::const_iterator iter = putPixels.begin(); iter != putPixels.end(); iter++)
		    m_surface->putPixel(*iter, colorkey);
	    }
	    if(!nonePyObject(pyObject.attr("drawvline"))) {
		std::vector<Rect> drawVLines = getPyObjectVector<Rect>(pyObject.attr("drawvline"));
	    	for (std::vector<Rect>::const_iterator iter = drawVLines.begin(); iter != drawVLines.end(); iter++)
		    m_surface->drawVLine((*iter).x, (*iter).y, (*iter).w, (*iter).h);
	    }
	    if(!nonePyObject(pyObject.attr("drawhline"))) {
		std::vector<Rect> drawHLines = getPyObjectVector<Rect>(pyObject.attr("drawhline"));
	    	for (std::vector<Rect>::const_iterator iter = drawHLines.begin(); iter != drawHLines.end(); iter++)
		    m_surface->drawHLine((*iter).x, (*iter).y, (*iter).w, (*iter).h);
	    }
	    if(!nonePyObject(pyObject.attr("fillrect"))) {
		std::vector<Rect> fillRects = getPyObjectVector<Rect>(pyObject.attr("fillrect"));
	    	for (std::vector<Rect>::const_iterator iter = fillRects.begin(); iter != fillRects.end(); iter++)
		    m_surface->fillRect(colorkey, *iter);
	    }
	}
	else {
	    LOG_ERROR("GameData", "%s is of type %s, must be of type GameDataConst or GameDataMod!", m_path.c_str());
	    exit(EXIT_FAILURE);
	}
	m_persistent = pyObject.attr("persistent");
    }
    catch(python::error_already_set const &) {
	LOG_FATAL("GameData", "Error loading data: %s", m_path.c_str());
	PyErr_Print();
	exit(EXIT_FAILURE);
    }
}

void GameData::loadSound() {
    Mix_Chunk* soundChunk;

    try {
        SDL_RWops *rwop;
        size_t len;
        uint8_t *data;
	std::string filename;
	python::object pyObject = DataCache::Instance()->getPyObject(m_path);


	if(!getPyObject(pyObject.attr("filename"), &filename)) {
	    LOG_ERROR("GameData", "%s: 'filename' variable missing!", filename.c_str());
	    exit(EXIT_FAILURE);
	    }

        data = ResMan::Instance()->readFile(filename, &len);
        if((rwop = SDL_RWFromMem(data, len)) ==NULL) {
            LOG_ERROR("GameData", "getChunkFromFile(): Cannot open %s!",filename.c_str());
            exit(EXIT_FAILURE);
        }

        if((soundChunk = LoadVOC_RW(rwop, 0)) == NULL) {
            LOG_ERROR("DataCache", "getChunkFromFile(): Cannot load %s!",filename.c_str());
            exit(EXIT_FAILURE);		
        }

        SDL_RWclose(rwop);
        free(data);
    	m_sound.reset(soundChunk);
    }
    catch(python::error_already_set const &) {
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
	loadSound();

    return m_sound;
}

