#include "Definitions.h"

#include <eastwood/CpsFile.h>
#include <eastwood/Exception.h>
#include <eastwood/IcnFile.h>
#include <eastwood/MapFile.h>
#include <eastwood/PalFile.h>
#include <eastwood/ShpFile.h>
#include <eastwood/VocFile.h>
#include <eastwood/SDL/Surface.h>
#include <eastwood/SDL/Mixer/Sound.h>

#include "GameData.h"

#include "DataCache.h"
#include "Gfx.h"
#include "Log.h"
#include "PythonObjects.h"
#include "ResMan.h"
#include "Sfx.h"

// set m_persistent to true to avoid it being freed
GameData::GameData(std::string path) : m_path(path), m_freeCounter(0), m_persistent(true)
{
}

GameData::~GameData()
{
}

bool GameData::freeIfUnique() {
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
	    LOG(LV_WARNING, "GameData", "%s has been freed over 50 times!", m_path.c_str());
	LOG(LV_DEBUG, "GameData", "Freeing %s", m_path.c_str());
	return true;
    }
    return false;
}

void GameData::drawImage()
{
    try {
	eastwood::IStream *data;
	std::string variable;
	int value;
	UPoint pos;
	eastwood::Palette palette;
	python::object pyObject = DataCache::Instance()->getPyObject("objects", m_path);

	if(getPyObject(pyObject.attr("palette"), &variable))
	    palette = DataCache::Instance()->getPalette(variable);
	else
	    LOG(LV_ERROR, "GameData", "No palette for %s!", m_path.c_str());

	if(getPyObjectType(pyObject, 1) == "GameDataConst") {
	    if(!getPyObject(pyObject.attr("filename"), &variable)) {
		LOG(LV_ERROR, "GameData", "%s: 'filename' variable missing!", m_path.c_str());
		exit(EXIT_FAILURE);
	    }

	    // TODO: autodetection would be nice..
	    std::string type;
	    if(!getPyObject(pyObject.attr("type"), &type))
		type = variable.substr(variable.length()-3, 3);

	    data = ResMan::Instance()->getFile(variable);

	    if (type == "CPS") {
		eastwood::CpsFile cpsfile(*data, palette);
		m_surface.reset(new Image(cpsfile.getSurface()));
	    }

	    if (type == "SHP") {
		std::vector<uint32_t> tiles = getPyObjectVector<uint32_t>(pyObject.attr("tiles"));
		eastwood::ShpFile shpfile(*data, palette);
		if(getPyObject<int>(pyObject.attr("index"), &value))
		    m_surface.reset(new Image(shpfile.getSurface(value)));
		else if(!tiles.empty()) {
		    uint32_t tilesX = 0, tilesY = 0;

		    if(!tilesX)
			tilesX = tiles.size();
		    else if(tilesX != tiles.size())
			LOG(LV_FATAL, "GameData:", "Tile row size %d is of different size than %d for %s!",
				tiles.size(), tilesX, m_path.c_str());
		    tilesY++;
		    m_surface.reset(new Image(shpfile.getSurfaceArray(tilesX, tilesY, &tiles.front())));
		}
		else {
		    LOG(LV_FATAL, "GameData", "%s: No index or tiles specified!", m_path.c_str());
		    exit(EXIT_FAILURE);
		}
	    }
	    if (type == "ICN") {
		std::string mapName;
		if(getPyObject<std::string>(pyObject.attr("map"), &mapName)) {
		    data = ResMan::Instance()->getFile(mapName);
		    eastwood::MapFile map(*data);		    

		    eastwood::IcnFile icnfile(*data, map, palette);
    		    if(getPyObject<int>(pyObject.attr("index"), &value))
			m_surface.reset(new Image(icnfile.getSurface(value)));
		    else if(getPyObject<UPoint>(pyObject.attr("row"), &pos))
			m_surface.reset(new Image(icnfile.getTiles(pos.x, pos.y)));
/*		    else if(getPyObject<int>(pyObject.attr("mapindex"), &value)) {
			int tilesN = python::extract<int>(pyObject.attr("num"));
			UPoint tilePos = python::extract<UPoint>(pyObject.attr("tilepos"));
			m_surface.reset(new Image(icnfile.getSurfaceArray(value, tilePos.x, tilePos.y, tilesN)));
		    }*/
		    else {
			LOG(LV_FATAL, "GameData", "no index, mapindex or row specified for %s!", m_path.c_str());
			exit(EXIT_FAILURE);
		    }
		}
		else {
		    LOG(LV_FATAL, "GameData", "No map specified for %s!", m_path.c_str());
		    exit(EXIT_FAILURE);
		}
	    }
	}
	else if(getPyObjectType(pyObject, 1) == "GameDataMod") {
	    uint32_t colorkey = 0;
	    Rect crop;
	    ImagePtr gameData;
	    if((variable = getPyObjectType(pyObject.attr("gamedata"), 0)) != "NoneType")
		gameData = DataCache::Instance()->getGameData(variable)->getImage();
	    else {
		LOG(LV_ERROR, "GameMan", "%s: gamedata variable missing!", m_path.c_str());
		exit(EXIT_FAILURE);
	    }
	    if(getPyObject(pyObject.attr("crop"), &crop))
		m_surface = gameData->getPictureCrop(crop);
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
		    m_surface->drawVLine(UPoint((*iter).x, (*iter).y), (*iter).w, (*iter).h);
	    }
	    if(!nonePyObject(pyObject.attr("drawhline"))) {
		std::vector<Rect> drawHLines = getPyObjectVector<Rect>(pyObject.attr("drawhline"));
	    	for (std::vector<Rect>::const_iterator iter = drawHLines.begin(); iter != drawHLines.end(); iter++)
		    m_surface->drawHLine(UPoint((*iter).x, (*iter).y), (*iter).w, (*iter).h);
	    }
	    if(!nonePyObject(pyObject.attr("fillrect"))) {
		std::vector<Rect> fillRects = getPyObjectVector<Rect>(pyObject.attr("fillrect"));
	    	for (std::vector<Rect>::const_iterator iter = fillRects.begin(); iter != fillRects.end(); iter++)
		    m_surface->fillRect(colorkey, *iter);
	    }
	}
	else {
	    LOG(LV_ERROR, "GameData", "%s is of type %s, must be of type GameDataConst or GameDataMod!", m_path.c_str());
	    exit(EXIT_FAILURE);
	}
	//m_persistent = pyObject.attr("persistent");
    }
    catch(python::error_already_set const &) {
	LOG(LV_FATAL, "GameData", "Error loading data: %s", m_path.c_str());
	PyErr_Print();
	throw;
    } catch(eastwood::Exception e) {
	LOG(LV_ERROR, "GameData::drawImage()", "%S %S: %S", &((ConstString)m_path), &((ConstString)e.getLocation()), &((ConstString)e.getMessage()));
	throw;
    }
    catch(std::out_of_range e) {
	LOG(LV_ERROR, "GameData::drawImage()", "%S %s", &((ConstString)m_path), e.what());
	throw;
    }

}

void GameData::loadSound() {

    try {
	Sound sound;
	eastwood::IStream *data;
	std::string filename;
	python::object pyObject = DataCache::Instance()->getPyObject("objects", m_path);


	if(!getPyObject(pyObject.attr("filename"), &filename)) {
	    LOG(LV_ERROR, "GameData::loadSound()", "%s: 'filename' variable missing!", filename.c_str());
	    exit(EXIT_FAILURE);
	    }

        data = ResMan::Instance()->getFile(filename);
	sound = eastwood::SDL::Mixer::Sound(eastwood::VocFile(*data).getSound()).getResampled(eastwood::I_LINEAR);

    	m_sound.reset(new Sound(sound));
    }
    catch(python::error_already_set const &) {
	LOG(LV_FATAL, "GameData::loadSound()", "Error loading sound: %s", m_path.c_str());
	PyErr_Print();
	exit(EXIT_FAILURE);
    } catch(eastwood::Exception e) {
	LOG(LV_ERROR, "GameData::loadSound()", "%S %S: %S", &((ConstString)m_path), &((ConstString)e.getLocation()), &((ConstString)e.getMessage()));
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
    if (iter != m_remappedImg.end() && iter->second)
	return m_remappedImg.find(house)->second;
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

