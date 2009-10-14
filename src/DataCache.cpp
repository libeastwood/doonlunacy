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
#include <eastwood/SDL/Surface.h>

#include <string>

#include <boost/format.hpp>


DataCache::DataCache() {
}

#include <iostream>

void DataCache::Init(){
    eastwood::IStream *data;
    //    Image *tmp;

    ResMan::Instance()->addRes("ENGLISH");

    // Not properly decoded yet..
    // CreditsStrings = new StringFile("ENGLISH:CREDITS.ENG");

    data = ResMan::Instance()->getFile("ENGLISH:INTRO.ENG");
    IntroStrings = new eastwood::StringFile(*data);

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

    data = ResMan::Instance()->getFile("ENGLISH:TEXTA.ENG");	
    BriefingStrings[0] = new eastwood::StringFile(*data);
    data = ResMan::Instance()->getFile("ENGLISH:TEXTO.ENG");
    BriefingStrings[1] = new eastwood::StringFile(*data);
    data = ResMan::Instance()->getFile("ENGLISH:TEXTH.ENG");
    BriefingStrings[2] = new eastwood::StringFile(*data);
}

python::object DataCache::loadPyObject(std::string moduleName, std::string objectName) {
    python::object main = python::import("__main__").attr("__dict__");
    python::object module = python::import((python::str)moduleName).attr("__dict__");
    python::dict objects = python::extract<python::dict>(module["objects"]);

    if(objects.has_key(objectName))
	return python::eval(((std::string)python::extract<std::string>(objects[objectName].attr("__name__")) + "()").c_str(), main, module);

    return python::eval(((std::string)objectName + "()").c_str(), main, module);
}

eastwood::Palette DataCache::getPalette(std::string paletteFile)
{
    if(m_palette.find(paletteFile) == m_palette.end())
    {
	eastwood::IStream *data = ResMan::Instance()->getFile(paletteFile);
        m_palette[paletteFile] = eastwood::PalFile(*data).getPalette();
    }
    return m_palette[paletteFile];
}


song *DataCache::getMusic(MUSICTYPE musicType, uint16_t ID)
{
    std::string mtype;
    if(musicType == MUSIC_ATTACK)
	mtype = "attack";
    else if(musicType == MUSIC_LOSE)
	mtype = "lose";
    else if(musicType == MUSIC_PEACE)
	mtype = "peace";
    else if(musicType == MUSIC_WIN)
	mtype = "win";
    else if(musicType == MUSIC_INTRO)
	mtype = "intro";
    python::dict music = python::extract<python::dict>(python::import("music").attr("music"));
    song * newSong = new song;

    std::string filename = python::extract<std::string>(music[mtype][ID][0]);
    int track = python::extract<int>(music[mtype][ID][1]);

    newSong->filename = filename;
    newSong->track = track;

    return newSong;
}

std::string	DataCache::getBriefingText(uint16_t mission, eastwood::MissionType textType, HOUSETYPE house) {
    return BriefingStrings[house]->getString(mission,textType);
}

std::string	DataCache::getIntroString(uint16_t i){
    return IntroStrings->getString(i);
}

std::string	DataCache::getCreditsString(uint16_t i){
    return CreditsStrings->getString(i);
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
    for(std::map<std::string, GameData*>::const_iterator iter = m_gameData.begin(); iter != m_gameData.end(); iter++)
        iter->second->freeIfUnique();
}

AnimationLabel *DataCache::getAnimationLabel(std::string path)
{
    AnimationLabel* animationLabel = new AnimationLabel();

    try
    {
	eastwood::IStream *data;
	float frameRate;

        std::string variable, type;
	eastwood::Palette palette;
	python::object pyObject = DataCache::Instance()->getPyObject("objects", path);

	if(::getPyObject(pyObject.attr("palette"), &variable))
	    palette = DataCache::Instance()->getPalette(variable);
	else
	    LOG(LV_ERROR, "DataCache", "No palette for %s!", path.c_str());

	if(!::getPyObject(pyObject.attr("filename"), &variable)) {
	    LOG(LV_ERROR, "DataCache", "%s: 'filename' variable missing!", path.c_str());
	    exit(EXIT_FAILURE);
	}

	data = ResMan::Instance()->getFile(variable);

	type = variable.substr(variable.length()-3, 3);

        if (type == "WSA") {
	    eastwood::WsaFile wsafile(*data, palette);

            for(uint16_t i = 0; i < wsafile.size(); i++)
                animationLabel->addFrame(ImagePtr(new Image(wsafile.getSurface(i))));
        }

        if (type == "SHP") {
	    UPoint index;
	    eastwood::ShpFile shpfile(*data, palette);
	    
    	    if(!::getPyObject(pyObject.attr("index"), &index)) {
    		LOG(LV_ERROR, "DataCache", "%s: 'index' variable missing!", path.c_str());
    		exit(EXIT_FAILURE);
    	    }

            for(uint16_t i = index.x; i < index.y; i++)
                animationLabel->addFrame(ImagePtr(new Image(shpfile.getSurface(i))));
        }
	if(::getPyObject(pyObject.attr("framerate"), &frameRate))
            animationLabel->setFrameRate(frameRate);
    }
    catch(python::error_already_set const &) {
	LOG(LV_FATAL, "DataCache", "Error loading data: %s", path.c_str());
	PyErr_Print();
	exit(EXIT_FAILURE);
    }

    return animationLabel;
}

DataCache::~DataCache() {

}
