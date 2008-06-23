#include "boost/bind.hpp"

#include "Application.h"
#include "Editor.h"
#include "Gfx.h"
#include "MainMenu.h"
#include "OptionsMenu.h"
#include "ResMan.h"
#include "SingleMenu.h"
#include "Settings.h"
#include "SoundPlayer.h"

#include "pakfile/Wsafile.h"

MainMenuState::MainMenuState()
{
    //m_menuBackground = (SDL_Surface*)(dataFile[UI_Menu].dat); 
    //m_menu = new Window();
    //m_menu->setHeight(m_menuBackground->h);
    //m_menu->setWidth(m_menuBackground->w);

    m_vbox = new VBox();

    const int bw = 200;
    const int bh = 24;
    
    m_butSingle = new BoringButton("Single Player");
    m_butSingle->setSize(SPoint(bw, bh));
    //m_butSingle = new GraphicButton((SDL_Surface*)(dataFile[UI_Single].dat),
    //                   (SDL_Surface*)(dataFile[UI_Single_Pressed].dat));
    m_butSingle->onClick.connect(
            boost::bind(&MainMenuState::doSingle, this) );
   
    m_vbox->addChild(m_butSingle);

    m_butMulti = new BoringButton("Multi Player");
    m_butMulti->setSize(SPoint(bw, bh));
    //m_butMulti = new GraphicButton((SDL_Surface*)(dataFile[UI_Multi].dat),
    //                   (SDL_Surface*)(dataFile[UI_Multi_Pressed].dat));
    m_butMulti->onClick.connect(
            boost::bind(&MainMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butMulti);

    m_butMapEd = new BoringButton("Map Editor");
    m_butMapEd->setSize(SPoint(bw, bh));
    //m_butMapEd = new GraphicButton((SDL_Surface*)(dataFile[UI_MapEdit].dat),
    //                   (SDL_Surface*)(dataFile[UI_MapEdit_Pressed].dat));
    m_butMapEd->onClick.connect(
            boost::bind(&MainMenuState::doEditor, this) );
   
    m_vbox->addChild(m_butMapEd);

    m_butOptions = new BoringButton("Options");
    m_butOptions->setSize(SPoint(bw, bh));
    //m_butOptions = new GraphicButton((SDL_Surface*)(dataFile[UI_OptionsMM].dat),
    //                   (SDL_Surface*)(dataFile[UI_OptionsMM_Pressed].dat));
    m_butOptions->onClick.connect(
            boost::bind(&MainMenuState::doOptions, this) );
   
    m_vbox->addChild(m_butOptions);

    m_butAbout = new BoringButton("About");
    m_butAbout->setSize(SPoint(bw, bh));
    //m_butAbout = new GraphicButton((SDL_Surface*)(dataFile[UI_About].dat),
    //                   (SDL_Surface*)(dataFile[UI_About_Pressed].dat));
    m_butAbout->onClick.connect(
            boost::bind(&MainMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butAbout);

    m_butQuit = new BoringButton("Quit");
    m_butQuit->setSize(SPoint(bw, bh));
    //m_butQuit = new GraphicButton((SDL_Surface*)(dataFile[UI_Quit].dat),
    //                   (SDL_Surface*)(dataFile[UI_Quit_Pressed].dat));
    m_butQuit->onClick.connect(
            boost::bind(&MainMenuState::doQuit, this) );
   
    m_vbox->addChild(m_butQuit);

    m_vbox->fit(2);
    Uint16 x = (Settings::Instance()->GetWidth() / 2) - 
                (m_vbox->w / 2);
	m_vbox->setPosition(UPoint(x - 5, 312));
    m_vbox->reshape();
    
    m_container->addChild(m_vbox);

//TODO:I guess that most of this should be wrapped into some function as
//we'll be doing a lot of image cropping like this

    int len;
    unsigned char * data = ResMan::Instance()->readFile("FINALE:MAPPLAN.CPS", &len);

//TODO(proyvind): This isn't all that pretty right now, more a proof of concept
//                based on work done previously with imagemagick.
//                This should all be moved to some separate class with standard
//                functions for drawing frames etc..
//                
    CpsfilePtr m_cps (new Cpsfile(data, len));

    ImagePtr tmp (m_cps->getPicture());
    m_surf.reset(m_cps->getPicture());
    m_surf->blitFromCentered(tmp.get());

    data = ResMan::Instance()->readFile("DUNE:FAME.CPS", &len);
    m_cps.reset(new Cpsfile(data, len));
    tmp.reset(m_cps->getPicture());
    m_surf->blitFrom(tmp.get(), Rect(64, 9, 8, 8), UPoint(0, 0)); // nw corner
    m_surf->putPixel(UPoint(6,7), 0); // Edge smoothing..
    m_surf->putPixel(UPoint(7,6), 0);
    m_surf->putPixel(UPoint(7,7), 0);
    
    m_surf->blitFrom(tmp.get(), Rect(64,23, 8, 8), UPoint(312, 0)); // ne
    m_surf->putPixel(UPoint(312,6), 0); // Edge smoothing..
    m_surf->putPixel(UPoint(312,7), 0);
    m_surf->putPixel(UPoint(313,7), 0);

    m_surf->blitFrom(tmp.get(), Rect(248, 9, 8, 8), UPoint(0, 192)); // sw
    m_surf->putPixel(UPoint(6,192), 0); // Edge smoothing..
    m_surf->putPixel(UPoint(7,192), 0);
    m_surf->putPixel(UPoint(7,193), 0);
    
    m_surf->blitFrom(tmp.get(), Rect(248, 23, 8, 8), UPoint(312, 192)); // se
    m_surf->putPixel(UPoint(312,192), 0); // Edge smoothing..
    m_surf->putPixel(UPoint(312,193), 0);
    m_surf->putPixel(UPoint(313,192), 0);
    
    m_surf->blitFrom(tmp.get(), Rect(12, 73, 50, 4), UPoint(8, 0)); // top border
    m_surf->blitFrom(tmp.get(), Rect(12, 73, 50, 4), UPoint(58, 0));
    m_surf->blitFrom(tmp.get(), Rect(12, 73, 50, 4), UPoint(108, 0));
    m_surf->blitFrom(tmp.get(), Rect(12, 73, 50, 4), UPoint(158, 0));
    m_surf->blitFrom(tmp.get(), Rect(12, 73, 50, 4), UPoint(208, 0));
    m_surf->blitFrom(tmp.get(), Rect(12, 73, 50, 4), UPoint(258, 0));
    m_surf->blitFrom(tmp.get(), Rect(12, 73, 50, 4), UPoint(262, 0));

    m_surf->blitFrom(tmp.get(), Rect(12, 73, 50, 4), UPoint(8, 196)); // bottom border
    m_surf->blitFrom(tmp.get(), Rect(12, 73, 50, 4), UPoint(58, 196));
    m_surf->blitFrom(tmp.get(), Rect(12, 73, 50, 4), UPoint(108, 196));
    m_surf->blitFrom(tmp.get(), Rect(12, 73, 50, 4), UPoint(158, 196));
    m_surf->blitFrom(tmp.get(), Rect(12, 73, 50, 4), UPoint(208, 196));
    m_surf->blitFrom(tmp.get(), Rect(12, 73, 50, 4), UPoint(258, 196));
    m_surf->blitFrom(tmp.get(), Rect(12, 73, 50, 4), UPoint(262, 196));


    m_surf->blitFrom(tmp.get(), Rect(1, 83, 4, 50), UPoint(0, 8)); // left border
    m_surf->blitFrom(tmp.get(), Rect(1, 83, 4, 50), UPoint(0, 58));
    m_surf->blitFrom(tmp.get(), Rect(1, 83, 4, 50), UPoint(0, 108));
    m_surf->blitFrom(tmp.get(), Rect(1, 83, 4, 50), UPoint(0, 142));

    m_surf->blitFrom(tmp.get(), Rect(1, 83, 4, 50), UPoint(316, 8)); // right border
    m_surf->blitFrom(tmp.get(), Rect(1, 83, 4, 50), UPoint(316, 58));
    m_surf->blitFrom(tmp.get(), Rect(1, 83, 4, 50), UPoint(316, 108));
    m_surf->blitFrom(tmp.get(), Rect(1, 83, 4, 50), UPoint(316, 142));

}

MainMenuState::~MainMenuState()
{
    delete m_butSingle;
    delete m_butMulti;
    delete m_butMapEd;
    delete m_butOptions;
    delete m_butAbout;
    delete m_butQuit;

    delete m_vbox;
}

void MainMenuState::doEditor()
{
    mp_parent->PushState( new EditorState() );
}

void MainMenuState::doOptions()
{
    mp_parent->PushState( new OptionsMenuState() );
}

void MainMenuState::doSkirmish()
{
    printf("skirmish!\n");
    SoundPlayer::Instance()->playMusic(MUSIC_PEACE, 3);
    
#if 0
    Rect rect(0,0,30, 30);
    
    Image * img = new Image(UPoint(600, 300));
    int colour = 0;
    
    for (int j=0; j< 10; j++)
    {
        for (int i=0; i< 20; i++)
        {
            rect.x = 30 * i;
            rect.y = 30 * j;
            img->fillRect(colour, rect);
            colour++;
        }
    }
    SDL_SaveBMP(img->getSurface(), "palette.bmp");

#endif 
}

void MainMenuState::doSingle()
{
    mp_parent->PushState( new SingleMenuState() );
}

void MainMenuState::doQuit()
{
    PopState();
}

/*
void MainMenuState::JustMadeActive()
{
    Application::Instance()->RootWidget()->addChild(m_vbox);
    State::JustMadeActive();
}

void MainMenuState::JustMadeInactive()
{
    Application::Instance()->RootWidget()->deleteChild(m_vbox);
    State::JustMadeInactive();
}*/


int MainMenuState::Execute(float dt)
{
	if(!Mix_GetMusicHookData()){
		SoundPlayer::Instance()->playMusic(MUSIC_PEACE, 10);
	}
	m_surf->blitToScreen(SPoint(Settings::Instance()->GetWidth() / 2 - m_surf->getSurface()->w/2,
				Settings::Instance()->GetHeight() / 4));
	return 0;
}
