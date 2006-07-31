#include "Settings.h"
#include <fstream>

SETTINGSTYPE settings;

Settings::Settings()
{
  m_width = 640;
  m_height = 480;

  m_fullscreen = false;

  m_debug = false;

  m_dataDir = "paks/";

  m_doubleBuffered = false;
};

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
};

void Settings::ParseOptions(int argc, char* argv[])
{

};
