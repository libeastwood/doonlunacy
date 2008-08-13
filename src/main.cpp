#include "Application.h"
#include "Settings.h"
#include "states/MainMenu.h"
#include "states/CutSceneState2.h"

namespace boost {

	void throw_exception(std::exception const & e)
	{

	}

}

int main(int argc, char *argv[])
{
    Settings::Instance()->ParseFile("dunelegacy.cfg");
    Settings::Instance()->ParseOptions(argc, argv);

    Application::Instance()->Init();
    Application::Instance()->RootState()->PushState( new MainMenuState() );
    if(Settings::Instance()->GetPlayIntro())
	    Application::Instance()->RootState()->PushState( new CutSceneState("intro") );
    Application::Instance()->Run();
    Application::Destroy();
    Settings::Destroy();

    return 0;
}
