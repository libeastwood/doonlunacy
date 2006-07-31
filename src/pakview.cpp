#include "Application.h"
#include "Settings.h"

namespace boost {

	void throw_exception(std::exception const & e)
	{

	};

}

int main(int argc, char *argv[])
{
    Settings::Instance()->ParseFile("dunelegacy.cfg");
    Settings::Instance()->ParseOptions(argc, argv);

    Application::Instance()->Init();
    //Application::Instance()->PushState( new MainMenuState() );
    //Application::Instance()->PushState( new IntroState() );


    
    Application::Instance()->Run();
    
    Application::Destroy();
    Settings::Destroy();

    return 0;
};
