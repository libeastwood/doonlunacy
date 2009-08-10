#include <SDL_mixer.h>
#include <boost/python.hpp>
#include <boost/filesystem/path.hpp>

#include <iostream>
#include "Application.h"
#include "Point.h"
#include "Settings.h"
#include "Strings.h"
#include "ResMan.h"
#include "SoundPlayer.h"


SETTINGSTYPE settings;

Settings::Settings()
{

    // for config files 
    // set to the current directory for the moment 
#ifdef _WIN32
    boost::filesystem::path doonHome(std::getenv("HOMEDRIVE"));
    doonHome /= std::getenv("HOMEPATH");
    doonHome /= "Doon Lunacy";
#else
    boost::filesystem::path doonHome(std::getenv("HOME"));
    doonHome /= ".doonlunacy";
#endif
    m_configDir = doonHome.string();
    ResMan::Instance()->addRes("CONFIG", new WritableDIRResource(m_configDir, true) );

    //FIXME:Should this be in config file?? And should this be set for each game seperately??
    m_maxPathSearch = 100;
    m_updated = false;
}


void Settings::load()
{
    const char* settingsFile = "CONFIG:config.py";
    try{
        main = python::import("__main__");
        python::object global = main.attr("__dict__");
        local = python::dict();

        if (ResMan::Instance()->exists(settingsFile))
        {
            python::object result = python::exec_file(ResMan::Instance()->getRealPath(settingsFile).c_str(), global, local);
            m_dataDir = python::extract<std::string>(local["config"]["data_dir"]);
            m_debug = python::extract<int>(local["config"]["debug"]);
            m_gameSpeed = python::extract<int>(local["config"]["game_speed"]);
            m_gameDir = python::extract<std::string>(local["config"]["game_dir"]);

            m_playIntro = python::extract<bool>(local["config"]["play_intro"]);

            m_resolution.x = python::extract<int>(local["config"]["graphics"]["width"]);	
            m_resolution.y = python::extract<int>(local["config"]["graphics"]["height"]);
            m_fullscreen = python::extract<bool>(local["config"]["graphics"]["fullscreen"]);
            m_doubleBuffered = python::extract<bool>(local["config"]["graphics"]["double_buffered"]);

            m_soundOn = python::extract<bool>(local["config"]["sound"]["sound_on"]);
            m_sfxVolume = python::extract<int>(local["config"]["sound"]["sound_volume"]);
            m_responseVolume = python::extract<int>(local["config"]["sound"]["response_volume"]);
            m_voiceVolume = python::extract<int>(local["config"]["sound"]["voice_volume"]);
            m_musicOn = python::extract<bool>(local["config"]["sound"]["music_on"]);
            m_musicVolume = python::extract<int>(local["config"]["sound"]["music_volume"]);
            m_emuOpl = python::extract<int>(local["config"]["sound"]["opl_emulator"]);
        } else
        {
            python::exec("config = {'graphics' : dict(), 'sound' : dict()}", global, local);
            local["config"]["data_dir"] = m_dataDir = "DUNE2";
            local["config"]["debug"] = m_debug = 8;
            local["config"]["game_speed"] = m_gameSpeed = 4;
	    local["config"]["game_dir"] = m_gameDir = ".";
            local["config"]["play_intro"] = m_playIntro = true;

            local["config"]["graphics"]["width"] = m_resolution.x = 640;
            local["config"]["graphics"]["height"] = m_resolution.y = 480;
            local["config"]["graphics"]["fullscreen"] = m_fullscreen = false;
            local["config"]["graphics"]["double_buffered"] = m_doubleBuffered = true;

            local["config"]["sound"]["sound_on"] = m_soundOn = true;
            local["config"]["sound"]["sound_volume"] = m_sfxVolume = MIX_MAX_VOLUME/2;
            local["config"]["sound"]["response_volume"] = m_responseVolume = 100;
            local["config"]["sound"]["voice_volume"] = m_voiceVolume = 128;
            local["config"]["sound"]["music_on"] = m_musicOn = true;
            local["config"]["sound"]["music_volume"] = m_musicVolume = MIX_MAX_VOLUME/2;
            local["config"]["sound"]["opl_emulator"] = m_emuOpl = (int)CT_EMUOPL;
            m_updated = true;
	}
	boost::filesystem::path pythonData(GetGameDir());
	pythonData /= "python";

	std::string pythonPath = pythonData.string();
#ifdef _WIN32
	pythonPath += ";";
#else
	pythonPath += ":";
#endif
	pythonPath += Py_GetPath();

	PySys_SetPath((char*)pythonPath.c_str());
    }
    catch(python::error_already_set const &)
    {
        PyErr_Print();
        exit(EXIT_FAILURE);
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
    if(m_updated) {
	try {
	    python::object settings = python::import("settings");
    	    python::object global = settings.attr("__dict__");
	    python::exec("confstring = printconf('config', config)", global, local);

	    std::string configText = python::extract<std::string>(local["confstring"]);

	    ResMan::Instance()->writeText("CONFIG:config.py", configText);
	}
	catch(python::error_already_set const &)
	{
	    PyErr_Print();
	}
    }
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
    local["config"]["sound"]["opl_emulator"] = m_emuOpl;
    m_updated = true;
    return (EMUOPL)m_emuOpl;

}

UPoint Settings::ToggleResolution() {
    UPoint resolution;

    if(m_resolution.x < 800)
	resolution = UPoint(800, 600);
    else if(m_resolution.x < 1024)
	resolution = UPoint(1024, 768);
    else
	resolution = UPoint(640, 480);

    SetResolution(resolution);

    return m_resolution;
}

void Settings::SetResolution(UPoint resolution) {
    m_resolution = resolution;
    local["config"]["graphics"]["width"] = resolution.x;
    local["config"]["graphics"]["height"] = resolution.y;

    Application::Instance()->InitVideo();

    m_updated = true;


}

bool Settings::ToggleFullscreen() {
    local["config"]["graphics"]["fullscreen"] = m_fullscreen = !m_fullscreen;

    Application::Instance()->InitVideo();

    m_updated = true;

    return m_fullscreen;
}

