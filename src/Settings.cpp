#include <SDL_mixer.h>
#include <boost/python.hpp>




#include "Settings.h"
#include "Strings.h"
#include "ResMan.h"
#include "SoundPlayer.h"


using namespace boost;
SETTINGSTYPE settings;

Settings::Settings()
{
	// for config files 
	// set to the current directory for the moment 
	ResMan::Instance()->addRes("CONFIG", new WritableDIRResource("") );

    //FIXME:Should this be in config file?? And should this be set for each game seperately??
    m_maxPathSearch = 100;
}


void Settings::load()
{
    const char* settingsfile = "CONFIG:config.py";
    python::object main = python::import("__main__");
    python::dict global(main.attr("__dict__"));
    python::dict local;

    if (ResMan::Instance()->exists(settingsfile))
    {
	try{
	    python::object result = python::exec_file("config.py", global, local);
	    m_dataDir = python::extract<std::string>(local["data_dir"]);
	    m_debug = python::extract<int>(local["debug"]);
	    m_gameSpeed = python::extract<int>(local["game_speed"]);
	    m_playIntro = python::extract<bool>(local["play_intro"]);

	    m_height = python::extract<int>(local["graphics"]["height"]);
	    m_width = python::extract<int>(local["graphics"]["width"]);	
	    m_fullscreen = python::extract<bool>(local["graphics"]["fullscreen"]);
	    m_doubleBuffered = python::extract<bool>(local["graphics"]["double_buffer"]);

	    m_soundOn = python::extract<bool>(local["sound"]["sound_on"]);
	    m_sfxVolume = python::extract<int>(local["sound"]["sound_volume"]);
	    m_responseVolume = python::extract<int>(local["sound"]["response_volume"]);
	    m_voiceVolume = python::extract<int>(local["sound"]["voice_volume"]);
	    m_musicOn = python::extract<bool>(local["sound"]["music_on"]);
	    m_musicVolume = python::extract<int>(local["sound"]["music_volume"]);
	    m_emuOpl = python::extract<int>(local["sound"]["opl_emulator"]);
	}
	catch(python::error_already_set const &)
	{
	    // print all other errors to stderr
	    PyErr_Print();
	}

    } else
    {
	local["data_dir"] = m_dataDir = "paks/";
	local["debug"] = m_debug= 8;
	local["game_speed"] = m_gameSpeed = 4;
	local["play_intro"] = m_playIntro = true;

	local["height"] = m_height = 480;
	local["width"] = m_width = 640;
	local["fullscreen"] = m_fullscreen = false;
	local["double_buffered"] = m_doubleBuffered = true;

	local["sound_on"] = m_soundOn = true;
	local["sound_volume"] = m_sfxVolume = MIX_MAX_VOLUME/2;
	local["response_volume"] = m_responseVolume = 100;
	local["voice_volume"] = m_voiceVolume = 128;
	local["music_on"] = m_musicOn = true;
	local["music_volume"] = m_musicVolume = MIX_MAX_VOLUME/2;
	local["opl_emulator"] = m_emuOpl = (int)CT_EMUOPL;
    }

    Log::Instance()->setDefaultVerbosity(LogVerbosity(m_debug));
    /*
    if (configFile->exists("log_verbosity"))
    {
        Setting& node = configFile->lookup(".log_verbosity");
        
        for (int j = 0; j < node.getLength(); j++)
        {
            std::string logSystem = node[j][0];
            int logVerbosity = node[j][1];

            Log::Instance()->setVerbosity(logSystem, (LogVerbosity)logVerbosity);
        }
    }*/




//    Log::Instance()->setVerbosity("UnitClass", LV_MAX);
//    Log::Instance()->setVerbosity("ObjectClass", LV_MAX);    
//    Log::Instance()->setVerbosity("TerrainClass", LV_MAX);    
//    Log::Instance()->setVerbosity("MapClass", LV_MAX);        
/*        root = configFile::loadFile((const char *)data);
	std::string config = ResMan::Instance()->readText("CONFIG:config.txt");
	printf("%s\n", config.c_str());

	config += "newline\n";
	ResMan::Instance()->writeText("CONFIG:config.txt", config);

*/
}

void Settings::save()
{
    //TODO: dump __repr__() of local to disk..
    
//    String configText = configFile::saveFile(configFile);
//	ResMan::Instance()->writeText("CONFIG:config.txt", configText);    
/*
    configFile->lookup(".graphics.width") =  m_width;
    configFile->lookup(".graphics.height") =  m_height;
    configFile->lookup(".graphics.fullscreen") = m_fullscreen;
    configFile->lookup(".graphics.double_buffer") = m_doubleBuffered;
    configFile->lookup(".debug") = m_debug;
    configFile->lookup(".game_speed") = m_gameSpeed;
    configFile->lookup(".play_intro") = m_playIntro;

    configFile->lookup(".data_dir") = m_dataDir;
    configFile->lookup(".sound.sound_on") = m_soundOn;
    configFile->lookup(".sound.sound_volume") = m_sfxVolume;
    configFile->lookup(".sound.response_volume") = m_responseVolume;
    configFile->lookup(".sound.voice_volume") = m_voiceVolume;        
    configFile->lookup(".sound.music_on") = m_musicOn;
    configFile->lookup(".sound.music_volume") = m_musicVolume;
    configFile->lookup(".sound.opl_emulator") = m_emuOpl;
    configFile->writeFile("config.txt");
    */
}

void Settings::ParseFile(const char* fn)
{
  /*
  ifstream file(fn);
  
  if (!file.is_open()) 
  {
    printf("could not open config file '%s'\n", fn);
    return;
  }

  std::string line;

  while (!file.eof())
  {
    getline(file, line);


  };
  */
}

void Settings::ParseOptions(int argc, char* argv[])
{

}

EMUOPL Settings::GetEmuOpl() {
	// Don't permit other values, in such case, default to CT_EMUOPL
	switch(m_emuOpl){
		case CT_EMUOPL:
		case C_EMUOPL:
		case CK_EMUOPL:
		case CREAL_EMUOPL:
		case CANAL_EMUOPL:
			break;
		default:
			m_emuOpl = CT_EMUOPL;
			break;
	}
	return (EMUOPL)m_emuOpl;
}

EMUOPL Settings::ToggleEmuOpl(){
	switch(m_emuOpl){
		case CT_EMUOPL:
			m_emuOpl = C_EMUOPL;
			break;
		case C_EMUOPL:
			m_emuOpl = CK_EMUOPL;
			break;
		case CK_EMUOPL:
			m_emuOpl = CREAL_EMUOPL;
			break;
		case CREAL_EMUOPL:
			m_emuOpl = CANAL_EMUOPL;
			break;
		case CANAL_EMUOPL:
			m_emuOpl = CT_EMUOPL;
			break;
	}
	SoundPlayer::Instance()->changeEmuOpl((EMUOPL)m_emuOpl);
	return (EMUOPL)m_emuOpl;

}
