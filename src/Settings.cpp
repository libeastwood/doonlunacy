#include <fstream>
#include <SDL_mixer.h>

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
	const char* settingsfile = "CONFIG:config.txt";

    // try loading config file
    if (ResMan::Instance()->exists(settingsfile))
    {
        String configText = ResMan::Instance()->readText(settingsfile);
        configFile = ConfigFile::loadFile(configText.c_str());
    }
    else
    {
        // just parse empty string...
        configFile = ConfigFile::loadFile("");
	};

    ConfigFile::bind(".graphics.width", configFile, m_width, 640);
    ConfigFile::bind(".graphics.height", configFile, m_height, 480);
    ConfigFile::bind(".graphics.fullscreen", configFile, m_fullscreen, false);
    ConfigFile::bind(".graphics.double_buffer", configFile, m_doubleBuffered, false);
    ConfigFile::bind(".debug", configFile, m_debug, int(LV_ERROR));
    ConfigFile::bind(".play_intro", configFile, m_playIntro, true);

    ConfigFile::bind(".data_dir", configFile, m_dataDir, String("paks/"));
    ConfigFile::bind(".sound.sound_on", configFile, m_soundOn, true);
    ConfigFile::bind(".sound.sound_volume", configFile, m_sfxVolume, MIX_MAX_VOLUME/2);
    ConfigFile::bind(".sound.response_volume", configFile, m_responseVolume, 100);
    ConfigFile::bind(".sound.voice_volume", configFile, m_voiceVolume, 128);        
    ConfigFile::bind(".sound.music_on", configFile, m_musicOn, true);
    ConfigFile::bind(".sound.music_volume", configFile, m_musicVolume, MIX_MAX_VOLUME/2);
    ConfigFile::bind(".sound.opl_emulator", configFile, m_emuOpl, (int)CT_EMUOPL);

    
    Log::Instance()->setDefaultVerbosity(LogVerbosity(m_debug));
    Log::Instance()->setVerbosity("UnitClass", LV_MAX);
    Log::Instance()->setVerbosity("ObjectClass", LV_MAX);    
//    Log::Instance()->setVerbosity("TerrainClass", LV_MAX);    
//    Log::Instance()->setVerbosity("MapClass", LV_MAX);        
/*        root = ConfigFile::loadFile((const char *)data);
	std::string config = ResMan::Instance()->readText("CONFIG:config.txt");
	printf("%s\n", config.c_str());

	config += "newline\n";
	ResMan::Instance()->writeText("CONFIG:config.txt", config);

*/
}
void Settings::save()
{
    String configText = ConfigFile::saveFile(configFile);
	ResMan::Instance()->writeText("CONFIG:config.txt", configText);    
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
			m_emuOpl = CT_EMUOPL;
			break;
	}
	SoundPlayer::Instance()->changeEmuOpl((EMUOPL)m_emuOpl);
	return (EMUOPL)m_emuOpl;

}
