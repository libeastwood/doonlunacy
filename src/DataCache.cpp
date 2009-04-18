#include "DataCache.h" 
#include "ResMan.h"
#include "Log.h"
#include "PythonObjects.h"


#include <boost/python/class.hpp>
#include <boost/python/converter/rvalue_from_python_data.hpp>
#include "GameData.h"

#include "gui2/Label.h"

#include <eastwood/IcnFile.h>
#include <eastwood/ShpFile.h>
#include <eastwood/WsaFile.h>

#include <eastwood/PalFile.h>
#include <eastwood/StringFile.h>
#include <eastwood/VocFile.h>

#include <string>

#include <boost/format.hpp>


using namespace libconfig;

typedef boost::shared_ptr<IcnFile> IcnfilePtr;
typedef boost::shared_ptr<ShpFile> ShpfilePtr;

DataCache::DataCache() {
}

#include <iostream>

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

    size_t len;
    uint8_t *data;
    //    Image *tmp;

    ResMan::Instance()->addRes("ENGLISH");

    // Not properly decoded yet..
    // CreditsStrings = new StringFile("ENGLISH:CREDITS.ENG");

    data = ResMan::Instance()->readFile("ENGLISH:INTRO.ENG", &len);
    IntroStrings = new StringFile(data);
    free(data);

    ResMan::Instance()->addRes("DUNE");

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

    data = ResMan::Instance()->readFile("ENGLISH:TEXTA.ENG", &len);	
    BriefingStrings[0] = new StringFile(data);
    free(data);
    data = ResMan::Instance()->readFile("ENGLISH:TEXTO.ENG", &len);	
    BriefingStrings[1] = new StringFile(data);
    free(data);
    data = ResMan::Instance()->readFile("ENGLISH:TEXTH.ENG", &len);	
    BriefingStrings[2] = new StringFile(data);
    free(data);
}

void DataCache::loadPyObject(std::string objectName) {
    python::object main = python::import("__main__").attr("__dict__");
    python::object gamedata = python::import("objects").attr("__dict__");
    python::dict objects = python::extract<python::dict>(gamedata["objects"]);

    if(objects.has_key(objectName))
	m_pyObjects[objectName] = python::eval(((std::string)python::extract<std::string>(objects[objectName].attr("__name__")) + "()").c_str(), main, gamedata);
    else
	m_pyObjects[objectName] = python::eval(((std::string)objectName + "()").c_str(), main, gamedata);
}

SDL_Palette* DataCache::getPalette(std::string paletteFile)
{
    if(m_palette.find(paletteFile) == m_palette.end())
    {
        size_t len;
        uint8_t *data = ResMan::Instance()->readFile(paletteFile, &len);
        m_palette[paletteFile] = PalfilePtr(new PalFile(data, len));
        free(data);
    }
    return m_palette[paletteFile]->getPalette();
}


song *DataCache::getMusic(MUSICTYPE musicType, uint16_t ID)
{

    Setting& node = m_dataConfig->lookup("music");
    song * newSong = new song;

    std::string filename = node[musicType][ID][0];
    int track = (int)node[musicType][ID][1];

    newSong->filename = filename;
    newSong->track = track;

    return newSong;
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

GameData * DataCache::getGameData(std::string name)
{
    std::map<std::string, GameData*>::iterator gcObj = m_gameData.find(name);
    GameData *ret;

    if(gcObj == m_gameData.end()) {
        m_gameData[name] = ret = new GameData(name);
    }
    else
	ret = gcObj->second;

    return ret;
}

void DataCache::freeGameData()
{
    for(std::map<std::string, GameData*>::const_iterator iter = m_gcObjs.begin(); iter != m_gcObjs.end(); iter++)
        iter->second->freeIfUnique();
}

AnimationLabel *DataCache::getAnimationLabel(std::string path)
{
    AnimationLabel* animationLabel = new AnimationLabel();

    try
    {
        size_t len;
	uint8_t *data;
	float frameRate;

        std::string variable, type;
        SDL_Palette* palette;
	python::object pyObject = DataCache::Instance()->getPyObject(path);

	if(::getPyObject(pyObject.attr("palette"), &variable))
	    palette = DataCache::Instance()->getPalette(variable);
	else
	    LOG_ERROR("DataCache", "No palette for %s!", path.c_str());

	if(!::getPyObject(pyObject.attr("filename"), &variable)) {
	    LOG_ERROR("DataCache", "%s: 'filename' variable missing!", path.c_str());
	    exit(EXIT_FAILURE);
	}

	data = ResMan::Instance()->readFile(variable, &len);

	type = variable.substr(variable.length()-3, 3);

        if (type == "WSA") {
            WsaFile wsafile(data, len, palette);

            for(Uint16 i = 0; i < wsafile.getNumFrames(); i++)
                animationLabel->addFrame(ImagePtr(new Image(wsafile.getSurface(i))));
        }

        if (type == "SHP") {
	    UPoint index;
            ShpFile shpfile(data, len, palette);
	    
    	    if(!::getPyObject(pyObject.attr("index"), &index)) {
    		LOG_ERROR("DataCache", "%s: 'index' variable missing!", path.c_str());
    		exit(EXIT_FAILURE);
    	    }

            for(Uint16 i = index.x; i < index.y; i++)
                animationLabel->addFrame(ImagePtr(new Image(shpfile.getSurface(i))));
        }
	if(::getPyObject(pyObject.attr("framerate"), &frameRate))
            animationLabel->setFrameRate(frameRate);
    }
    catch(python::error_already_set const &) {
	LOG_FATAL("DataCache", "Error loading data: %s", path.c_str());
	PyErr_Print();
	exit(EXIT_FAILURE);
    }

    return animationLabel;
}

DataCache::~DataCache() {

}
