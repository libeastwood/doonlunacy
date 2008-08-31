#include <fstream>
#include <SDL_mixer.h>

#include "Settings.h"
#include "Strings.h"
#include "ResMan.h"
#include "SoundPlayer.h"



using namespace libconfig;
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
	const char* settingsfile = "CONFIG:config.txt";
	configFile = new Config();
    if (ResMan::Instance()->exists(settingsfile))
    {
        configFile->readFile("config.txt");
    } else
    {

        Setting &root = configFile->lookup(".");
        root.add("data_dir", Setting::TypeString) = "paks/";
        root.add("debug", Setting::TypeInt) = 8;
        root.add("game_speed", Setting::TypeInt) = 4;
        root.add("play_intro", Setting::TypeBoolean) = true;        
        root.add("graphics", Setting::TypeGroup);
        root.add("sound", Setting::TypeGroup);    

        Setting& node = configFile->lookup(".graphics");
        node.add("height", Setting::TypeInt) = 480;
        node.add("width", Setting::TypeInt) = 640;
        node.add("fullscreen", Setting::TypeBoolean) = false;
        node.add("double_buffer", Setting::TypeBoolean) = true;
        
        Setting& node2 = configFile->lookup(".sound");
        node2.add("sound_on", Setting::TypeBoolean) = true;
        node2.add("sound_volume", Setting::TypeInt) = MIX_MAX_VOLUME/2;
        node2.add("response_volume", Setting::TypeInt) = 100;
        node2.add("voice_volume", Setting::TypeInt) = 128;
        node2.add("music_on", Setting::TypeBoolean) = true;
        node2.add("music_volume", Setting::TypeInt) = MIX_MAX_VOLUME/2;
        node2.add("opl_emulator", Setting::TypeInt) = (int)CT_EMUOPL;
        configFile->writeFile("config.txt");
    }

    configFile->lookupValue(".graphics.width", m_width);
    configFile->lookupValue(".graphics.height", m_height);
    configFile->lookupValue(".graphics.fullscreen", m_fullscreen);
    configFile->lookupValue(".graphics.double_buffer", m_doubleBuffered);
    configFile->lookupValue(".debug", m_debug);
    configFile->lookupValue(".game_speed", m_gameSpeed);
    configFile->lookupValue(".play_intro", m_playIntro);

    configFile->lookupValue(".data_dir", m_dataDir);
    configFile->lookupValue(".sound.sound_on", m_soundOn);
    configFile->lookupValue(".sound.sound_volume", m_sfxVolume);
    configFile->lookupValue(".sound.response_volume", m_responseVolume);
    configFile->lookupValue(".sound.voice_volume", m_voiceVolume);        
    configFile->lookupValue(".sound.music_on", m_musicOn);
    configFile->lookupValue(".sound.music_volume", m_musicVolume);
    configFile->lookupValue(".sound.opl_emulator", m_emuOpl);

    Log::Instance()->setDefaultVerbosity(LogVerbosity(m_debug));
    if (configFile->exists("log_verbosity"))
    {
        Setting& node = configFile->lookup(".log_verbosity");
        
        for (int j = 0; j < node.getLength(); j++)
        {
            std::string logSystem = node[j][0];
            int logVerbosity = node[j][1];

            Log::Instance()->setVerbosity(logSystem, (LogVerbosity)logVerbosity);
        }
    }




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
//    String configText = configFile::saveFile(configFile);
//	ResMan::Instance()->writeText("CONFIG:config.txt", configText);    
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
