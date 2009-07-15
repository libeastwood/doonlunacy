#include "DataCache.h"
#include "Gfx.h"
#include "Settings.h"
#include "GameData.h"
#include "PythonObjects.h"


int main(int argc, char *argv[])
{
    ::InitPython();
    Settings::Instance()->load();
    DataCache::Instance()->Init();

    for(int i = 1; i < argc; i++)
    	DataCache::Instance()->getGameData(argv[i])->getImage()->saveBMP(((std::string)argv[i]) + ".bmp");
    
    return 0;
}
