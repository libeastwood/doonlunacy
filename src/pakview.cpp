#include "Application.h"
#include "DataCache.h"
#include "Gfx.h"
#include "Settings.h"
#include "State.h"
#include "ResMan.h"
#include "pakview.h"
#include "pakfile/Palette.h"

#include "gui2/Button.h"
#include "houses.h"

#include "boost/bind.hpp"

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
            img = ObjImg_Terrain;
            house = HOUSE_ATREIDES;
            m_button = new BoringButton("Click me, bitte!");
            m_button->setSize(SPoint(160, 50));
            m_button->setPosition(SPoint(10, 10));
            m_button->setVisible(true);
            m_button->onClick.connect(
                boost::bind(&PakViewState::NextImg, this) );
            
            m_houseBut = new BoringButton("Change house");
            m_houseBut->setSize(SPoint(132, 50));
            m_houseBut->setPosition(SPoint(200, 10));
            m_houseBut->setVisible(true);
            m_houseBut->onClick.connect(
                boost::bind(&PakViewState::SwitchHouse, this) );
            
            m_test = DataCache::Instance()->getObjImg(img, house);

            Application::Instance()->RootWidget()->addChild(m_button);            
            Application::Instance()->RootWidget()->addChild(m_houseBut);

       };

       ~PakViewState()
       {
            delete m_button;
       };
       
       int Execute(float dt)
       {
           m_test->blitToScreenCentered();
           return 0;
       };

       void NextImg()
       {
           img++;
           
           if (img == NUM_OBJIMGS)
           img = 0;
           
           m_test = DataCache::Instance()->getObjImg(img, house);
       };
       
       void SwitchHouse()
       {
           house++;
           fprintf(stderr, "HOUSE CHANGED\n");
           if (house == NUM_HOUSES)
           house = 0;
           m_test = DataCache::Instance()->getObjImg(img, house);
       };

      virtual const char* GetName() { return "PakViewState"; }

    private:
       BoringButton* m_button, *m_houseBut;
       ImagePtr m_test;
       int img, house;
};

int main(int argc, char *argv[])
{
    Settings::Instance()->ParseFile("dunelegacy.cfg");
    Settings::Instance()->ParseOptions(argc, argv);

    Application::Instance()->Init();
    
    DataCache::Instance();
    
    //Application::Instance()->PushState( new MainMenuState() );
    //Application::Instance()->PushState( new IntroState() );
    
    Application::Instance()->RootState()->PushState( new PakViewState() );

    Application::Instance()->Run();
    
    Application::Destroy();
    Settings::Destroy();

    return 0;
};
