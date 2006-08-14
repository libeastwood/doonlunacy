#include "Settings.h"
#include <fstream>

#include "Strings.h"
#include "ResMan.h"

SETTINGSTYPE settings;

Settings::Settings()
{
	// for config files 
	// set to the current directory for the moment 
	ResMan::Instance()->addRes("CONFIG", new WritableDIRResource("") );

  m_width = 640;
  m_height = 480;

  m_fullscreen = false;

  m_debug = false;

  m_dataDir = "paks/";

  m_doubleBuffered = false;
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
    ConfigFile::bind(".debug", configFile, m_debug, true);
    ConfigFile::bind(".data_dir", configFile, m_dataDir, String("paks/"));

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

void Settings::ParseFile(char* fn)
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
