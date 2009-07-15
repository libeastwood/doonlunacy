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

    std::string gamedata = argv[1];
    ImagePtr img = DataCache::Instance()->getGameData(gamedata)->getImage();

    img->saveBMP(gamedata + ".bmp");
    
    return 0;
}
