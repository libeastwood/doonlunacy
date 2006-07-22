#include "Application.h"
#include "Settings.h"

int main(int argc, char *argv[])
{
    Settings::Instance()->ParseFile("dunelegacy.cfg");
    Settings::Instance()->ParseOptions(argc, argv);

    Application::Instance()->Init();
    Application::Instance()->Run();
    
    Application::Destroy();
    Settings::Destroy();

    return 0;
};
