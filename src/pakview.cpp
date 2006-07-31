#include "Application.h"
#include "Settings.h"
#include "State.h"
#include "ResMan.h"
#include "pakfile/Palette.h"

#include "gui2/Button.h"

namespace boost {

	void throw_exception(std::exception const & e)
	{

	};

}

class PakViewState : public State
{
    public:
       PakViewState()
       {
            int len;
            unsigned char* data = ResMan::Instance()->readFile("INTRO:INTRO.PAL", &len);
            
            Palettefile pal (data, len);

            Application::Instance()->SetPalette(pal.getPalette());

            m_button = new BoringButton("Test");
            m_button->setSize(100, 50);
            m_button->setPos(10, 10);
            m_button->setVisible(true);

            Application::Instance()->RootWidget()->addChild(m_button);

       };

       ~PakViewState()
       {
            delete m_button;
       };
       
       int Execute(float dt)
       {
           return 0;
       };

      virtual const char* GetName() { return "PakViewState"; }

    private:
       BoringButton* m_button;
};

int main(int argc, char *argv[])
{
    Settings::Instance()->ParseFile("dunelegacy.cfg");
    Settings::Instance()->ParseOptions(argc, argv);

    Application::Instance()->Init();
    //Application::Instance()->PushState( new MainMenuState() );
    //Application::Instance()->PushState( new IntroState() );
    
    Application::Instance()->RootState()->PushState( new PakViewState() );

    Application::Instance()->Run();
    
    Application::Destroy();
    Settings::Destroy();

    return 0;
};
