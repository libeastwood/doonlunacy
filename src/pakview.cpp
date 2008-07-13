#include "Application.h"
#include "DataCache.h"
#include "Gfx.h"
#include "Settings.h"
#include "State.h"
#include "ResMan.h"
#include "pakview.h"
#include "pakfile/Palette.h"

#include "gui2/Label.h"
#include "houses.h"

#include "boost/bind.hpp"

namespace boost {

	void throw_exception(std::exception const & e)
	{

	}

}

class PakViewState : public State
{
    public:
       PakViewState()
       {
            img = 0;
            house = 0;

            m_label = new Label("Use arrows (up,down,left,right) to switch houses and drawn sprites");
            m_label->setPosition(UPoint(10, 10));
            
            m_test = DataCache::Instance()->getObjPic((ObjPic_enum)img, HOUSETYPE(house));

            Application::Instance()->RootWidget()->addChild(m_label);            

       };

       ~PakViewState()
       {
            delete m_label;
       };
       
       int Execute(float dt)
       {
		   SDL_Event event;
		   SDL_PollEvent(&event);
		   if (event.type == SDL_KEYDOWN) {
			   switch (event.key.keysym.sym) {
				case SDLK_RIGHT : SwitchImg(1); break;
				case SDLK_LEFT : SwitchImg(-1); break;
				case SDLK_UP : SwitchHouse(+1); break;
				case SDLK_DOWN : SwitchHouse(-1); break;
				case SDLK_s: SDL_SaveBMP(m_test->getSurface(), "surface.bmp"); break;
				case SDLK_ESCAPE : Application::Instance()->Die(); break;
				default : break;
			   }
		   }
		 
           m_test->blitToScreenCentered();
           return 0;
       };

       void SwitchImg(int i)
       {
           img+=i;
           
           if (img == NUM_OBJPICS)
			   img = 0;
		   
		   if (img < 0)
			   img = NUM_OBJPICS-1;

           m_test = DataCache::Instance()->getObjPic((ObjPic_enum)img, HOUSETYPE(house));
       };
       
       void SwitchHouse(int i)
       {
           house+=i;
           fprintf(stderr, "HOUSE CHANGED\n");
           if (house == NUM_HOUSES)
			   house = 0;
		   if (house < 0)
			   house = NUM_HOUSES-1;

           m_test = DataCache::Instance()->getObjPic((ObjPic_enum)img, HOUSETYPE(house));
       };

      virtual const char* GetName() { return "PakViewState"; }

    private:
       Label * m_label;
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
}
