#include "DataCache.h" 
#include "ResMan.h"
#include "Log.h"

#include "GCObject.h"

#include "gui2/Label.h"

#include <eastwood/IcnFile.h>
#include <eastwood/ShpFile.h>
#include <eastwood/WsaFile.h>

#include <eastwood/PalFile.h>
#include <eastwood/StringFile.h>
#include <eastwood/VocFile.h>

#include <string>

#include <boost/format.hpp>

namespace python = boost::python;

using namespace libconfig;

typedef boost::shared_ptr<IcnFile> IcnfilePtr;
typedef boost::shared_ptr<ShpFile> ShpfilePtr;

DataCache::DataCache() {
}

//#define GROUNDUNIT_ROW(i) (i+2)|TILE_NORMAL,(i+1)|TILE_NORMAL,i|TILE_NORMAL,(i+1)|TILE_FLIPV,(i+2)|TILE_FLIPV,(i+3)|TILE_FLIPV, (i+4)|TILE_NORMAL,(i+3)|TILE_NORMAL

#include <iostream>
Uint32 *GROUNDUNIT_ROW(Uint32 i){
    Uint32 *row = new Uint32[8];
    row[0] = (i+2)|TILE_NORMAL, row[1] = (i+1)|TILE_NORMAL, row[2] = i|TILE_NORMAL, row[3] = (i+1)|TILE_FLIPV,
        row[4] = (i+2)|TILE_FLIPV, row[5] = (i+3)|TILE_FLIPV, row[6] = (i+4)|TILE_NORMAL, row[7] = (i+3)|TILE_NORMAL;
    return row;
}

//#define AIRUNIT_ROW(i) (i+2)|TILE_NORMAL,(i+1)|TILE_NORMAL,i|TILE_NORMAL,(i+1)|TILE_FLIPV,(i+2)|TILE_FLIPV,(i+1)|TILE_ROTATE, i|TILE_FLIPH,(i+1)|TILE_FLIPH

Uint32 *AIRUNIT_ROW(Uint32 i){
    Uint32 *row = new Uint32[8];
    row[0] = (i+2)|TILE_NORMAL, row[1] = (i+1)|TILE_NORMAL, row[2] = i|TILE_NORMAL, row[3] = (i+1)|TILE_FLIPV,
        row[4] = (i+2)|TILE_FLIPV, row[5] = (i+1)|TILE_ROTATE, row[6] = i|TILE_FLIPH, row[7] = (i+1)|TILE_FLIPH;
    return row;
}

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

    cacheSprites();


    soundChunk.resize(NUM_SOUNDCHUNK);
    size_t len;
    uint8_t *data;
    //    Image *tmp;

    ResMan::Instance()->addRes("ENGLISH");

    addPalette(WESTWOOD_PAL, "INTRO:WESTWOOD.PAL");
    // Not properly decoded yet..
    // CreditsStrings = new StringFile("ENGLISH:CREDITS.ENG");

    addPalette(INTRO_PAL, "INTRO:INTRO.PAL");
    data = ResMan::Instance()->readFile("ENGLISH:INTRO.ENG", &len);
    IntroStrings = new StringFile((const char*)data);
    free(data);

    ResMan::Instance()->addRes("DUNE");

    addPalette(BENE_PAL, "DUNE:BENE.PAL");
    addPalette(IBM_PAL, "DUNE:IBM.PAL");

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
    BriefingStrings[0] = new StringFile((const char*)data);
    free(data);
    data = ResMan::Instance()->readFile("ENGLISH:TEXTO.ENG", &len);	
    BriefingStrings[1] = new StringFile((const char*)data);
    free(data);
    data = ResMan::Instance()->readFile("ENGLISH:TEXTH.ENG", &len);	
    BriefingStrings[2] = new StringFile((const char*)data);
    free(data);
}

// TODO: Eventually we want to create the unit objects in python rather than in C++..
void DataCache::cacheSprites()
{
    try {
        python::object main = python::import("__main__");
        python::dict global(main.attr("__dict__"));
        python::dict local;
        python::object result = python::exec_file("python/units.py", global, local);
        
        // TODO: figure out how to use the iterator :p
        // python::object units = ((python::dict)local["units"]).iterkeys();
        python::dict units = python::extract<python::dict>(local["units"]);
        python::list keys = units.keys();
        while(keys)
        {
            sprite tmp;
            std::string key = python::extract<std::string>(keys.pop());
            python::dict unit = (python::dict)units[key];


            tmp.health = -1;
            if(unit.has_key("health"))
                tmp.health = python::extract<int>(unit["health"]);

            tmp.maxHealth = -1;
            if(unit.has_key("max_health"))
                tmp.maxHealth = python::extract<int>(unit["max_health"]);

            tmp.numWeapons = -1;
            if(unit.has_key("numWeapons"))
                tmp.numWeapons = python::extract<int>(unit["numWeapons"]);

            if(unit.has_key("object_class"))
                tmp.objectClass = python::extract<std::string>(unit["object_class"]);

            if(unit.has_key("pic"))
                tmp.pic = python::extract<std::string>(unit["pic"]);

            tmp.primaryWeaponReloadTime = -1;
            if(unit.has_key("primary_weapon_reload_time"))
                tmp.primaryWeaponReloadTime = python::extract<int>(unit["primary_weapon_reload_time"]);

            tmp.radius = -1;
            if(unit.has_key("radius"))
                tmp.radius = python::extract<int>(unit["radius"]);

            tmp.turnSpeed = -1;
            if(unit.has_key("turn_speed"))
                tmp.turnSpeed = python::extract<float>(unit["turn_speed"]);

            tmp.speed = -1;
            if(unit.has_key("speed"))
                tmp.radius = python::extract<float>(unit["speed"]);

            tmp.size = UPoint(-1,-1);
            if(unit.has_key("size"))
                tmp.size = UPoint(python::extract<int>(unit["size"][0]),
                            python::extract<int>(unit["size"][1]));

            tmp.viewRange = -1;
            if(unit.has_key("view_range"))
                tmp.radius = python::extract<int>(unit["view_range"]);

            tmp.weaponDamage = -1;
            if(unit.has_key("weapon_damage"))
                tmp.weaponDamage = python::extract<int>(unit["weapon_damage"]);

            tmp.weaponRange = -1;
            if(unit.has_key("weapon_range"))
                tmp.weaponRange = python::extract<int>(unit["weapon_range"]);

            m_sprites[key] = tmp;
            LOG_INFO("DataCache", "Cached info for %s", key.c_str());
        }
    }
    catch(python::error_already_set const &)
    {
        LOG_FATAL("DataCache", "Error loading sprites..");
        PyErr_Print();
    }
}

sprite* DataCache::getSpriteInfo(std::string spriteName)
{
    std::map<std::string, sprite>::iterator iter;

    iter = m_sprites.find(spriteName);

    if (iter != m_sprites.end())
    { 
        return &m_sprites.find(spriteName)->second;
    }
    else
    {
        LOG_ERROR("DataCache", "Info for sprite \"%s\" was not cached", spriteName.c_str());
        exit(1);
    }

}

void DataCache::addPalette(Palette_enum palette, std::string paletteFile)
{
    size_t len;
    uint8_t * data = ResMan::Instance()->readFile(paletteFile, &len);
    PalfilePtr tmp (new PalFile(data, len));
    free(data);

    m_palette[palette] = tmp;
    m_palStrings.insert(std::pair<std::string, PalfilePtr>(paletteFile, tmp));
}

SDL_Palette* DataCache::getPalette(Palette_enum palette)
{
    return m_palette[palette]->getPalette();
}

SDL_Palette* DataCache::getPalette(std::string paletteFile)
{
    return m_palStrings[paletteFile]->getPalette();
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

GCObject *DataCache::getGCObject(std::string path)
{
    GCObject *gcObj = NULL;
    if(!m_gcObjs.empty())
        for(uint32_t i = 0; i < m_gcObjs.size(); i++)
            if(m_gcObjs[i]->getPath() == path)
            {
                gcObj = m_gcObjs[i];
            }
    if(gcObj == NULL){
        gcObj = new GCObject(path);
        m_gcObjs.push_back(gcObj);
    }
    return gcObj;

}

void DataCache::freeGCObjects()
{
    for(uint32_t i = 0; i < m_gcObjs.size(); i++)
        m_gcObjs[i]->freeIfUnique();
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
            palette = getPalette(IBM_PAL);

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

// vim:ts=8:sw=4:et

