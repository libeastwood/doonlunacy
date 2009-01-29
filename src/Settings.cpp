#include <SDL_mixer.h>
#include <boost/python.hpp>




#include "Settings.h"
#include "Strings.h"
#include "ResMan.h"
#include "SoundPlayer.h"


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
    main = python::import("__main__");
    python::object global = main.attr("__dict__");
    python::object settings = python::import("settings");
    local = settings.attr("__dict__");

    try {
        if (ResMan::Instance()->exists(settingsfile))
        {
            python::object result = python::exec_file("config.py", global, local);
            m_dataDir = python::extract<std::string>(local["config"]["data_dir"]);
            m_debug = python::extract<int>(local["config"]["debug"]);
            m_gameSpeed = python::extract<int>(local["config"]["game_speed"]);
            m_playIntro = python::extract<bool>(local["config"]["play_intro"]);

            m_height = python::extract<int>(local["config"]["graphics"]["height"]);
            m_width = python::extract<int>(local["config"]["graphics"]["width"]);	
            m_fullscreen = python::extract<bool>(local["config"]["graphics"]["fullscreen"]);
            m_doubleBuffered = python::extract<bool>(local["config"]["graphics"]["double_buffer"]);

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
            local["config"]["data_dir"] = m_dataDir = "paks/";
            local["config"]["debug"] = m_debug= 8;
            local["config"]["game_speed"] = m_gameSpeed = 4;
            local["config"]["play_intro"] = m_playIntro = true;

            local["config"]["graphics"]["height"] = m_height = 480;
            local["config"]["graphics"]["width"] = m_width = 640;
            local["config"]["graphics"]["fullscreen"] = m_fullscreen = false;
            local["config"]["graphics"]["double_buffered"] = m_doubleBuffered = true;

            local["config"]["sound"]["sound_on"] = m_soundOn = true;
            local["config"]["sound"]["sound_volume"] = m_sfxVolume = MIX_MAX_VOLUME/2;
            local["config"]["sound"]["response_volume"] = m_responseVolume = 100;
            local["config"]["sound"]["voice_volume"] = m_voiceVolume = 128;
            local["config"]["sound"]["music_on"] = m_musicOn = true;
            local["config"]["sound"]["music_volume"] = m_musicVolume = MIX_MAX_VOLUME/2;
            local["config"]["sound"]["opl_emulator"] = m_emuOpl = (int)CT_EMUOPL;
        }
    }
    catch(python::error_already_set const &)
    {
        PyErr_Print();
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
    try {
        python::object global = main.attr("__dict__");
        python::object settings = python::import("settings");

        python::exec("confstring = printconf('config', config)", global, local);

        std::string configText = python::extract<std::string>(local["confstring"]);

        ResMan::Instance()->writeText("CONFIG:config.py", configText);
    }
    catch(python::error_already_set const &)
    {
        PyErr_Print();
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
    return (EMUOPL)m_emuOpl;

}

// vim:ts=8:sw=4:et
