#include "DataCache.h" 
#include "ResMan.h"
#include "Log.h"

#include <boost/python/converter/rvalue_from_python_data.hpp>
#include "GameData.h"
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

    loadPyObjects();


    soundChunk.resize(NUM_SOUNDCHUNK);
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

void DataCache::loadPyObjects()
{

    std::string key;
    try {
        python::object objectClass = python::import("objects");

        python::dict objects = python::extract<python::dict>(objectClass.attr("objects"));
        python::list keys = objects.keys();
        while(keys)
        {
            key = python::extract<std::string>(keys.pop());
            m_pyObjects[key] = objects[key];

            LOG_INFO("DataCache", "Loaded python object %s", key.c_str());
        }

        objectClass = python::import("gamedata");
        objects = python::extract<python::dict>(objectClass.attr("gamedata"));
        keys = objects.keys();
        while(keys)
        {
            key = python::extract<std::string>(keys.pop());
            m_pyObjects[key] = objects[key];

            LOG_INFO("DataCache", "Loaded python gamedata object %s", key.c_str());
        }

    }
    catch(python::error_already_set const &)
    {
        LOG_FATAL("DataCache", "Error loading python object: %s", key.c_str());
        PyErr_Print();
        exit(EXIT_FAILURE);
    }
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


Mix_Chunk* DataCache::getSoundChunk(std::string ID)
{

    std::string fullpath = "sounds.";
    fullpath+=ID;

    std::string fileName;

    Mix_Chunk* returnChunk;

    try
    {
        Setting& node = m_dataConfig->lookup(fullpath);
        node.lookupValue("filename", fileName);
        SDL_RWops* rwop;
        size_t len;
        uint8_t * data;

        data = ResMan::Instance()->readFile(fileName.c_str(), &len);
        if((rwop = SDL_RWFromMem(data, len)) ==NULL) 
        {
            LOG_ERROR("DataCache", "getChunkFromFile(): Cannot open %s!",fileName.c_str());
            exit(EXIT_FAILURE);
        }

        if((returnChunk = LoadVOC_RW(rwop, 0)) == NULL) 
        {
            LOG_ERROR("DataCache", "getChunkFromFile(): Cannot load %s!",fileName.c_str());
            exit(EXIT_FAILURE);		
        }

        SDL_RWclose(rwop);
        free(data);

    }
    catch(ParseException& ex)
    {
        LOG_FATAL("DataCache", "Setting not found %d: %s", 
                ex.getLine(), ex.getError());
    }

    return returnChunk;
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
    std::string fullpath = "animations.";
    fullpath+=path;

    AnimationLabel* animationLabel = new AnimationLabel();

    try
    {
        Setting& node = m_dataConfig->lookup(fullpath);

        size_t len;
        uint8_t *data;

        std::string fileName;

        SDL_Palette* palette;
        if(node.lookupValue("palette", fileName))
        {
            palette = getPalette(fileName);
        }
        else
            palette = getPalette("DUNE:IBM.PAL");

        node.lookupValue("filename", fileName);
        std::string type = fileName.substr(fileName.length()-3, 3);

        data = ResMan::Instance()->readFile(fileName, &len);


        if (type.compare("WSA") == 0)
        {
            WsaFile *wsafile(new WsaFile(data, len, palette));

            for(uint32_t i = 0; i < wsafile->getNumFrames(); i++)
                animationLabel->addFrame(ImagePtr(new Image(wsafile->getSurface(i))));

            float frameRate = 1.0;
            node.lookupValue("frame_rate", frameRate);
            animationLabel->setFrameRate(frameRate);

            delete wsafile;

        }

        if (type.compare("SHP") == 0)
        {
            uint32_t startIndex, endIndex;
            float frameRate = 1.0;
            node.lookupValue("start_index", startIndex);
            node.lookupValue("end_index", endIndex);

            ShpFile *shpfile = new ShpFile(data, len, palette);
            for(uint32_t i = startIndex; i < endIndex; i++)
                animationLabel->addFrame(ImagePtr(new Image(shpfile->getSurface(i))));

            node.lookupValue("frame_rate", frameRate);
            animationLabel->setFrameRate(frameRate);

            delete shpfile;

        }

    }
    catch(ParseException& ex)
    {
        LOG_FATAL("DataCache", "Setting not found %d: %s", 
                ex.getLine(), ex.getError());

        exit(EXIT_FAILURE);
    }


    return animationLabel;
}

DataCache::~DataCache() {

}
