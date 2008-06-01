#include "boost/bind.hpp"

#include "Application.h"
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
            boost::bind(&MainMenuState::doSkirmish, this) );
   
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
    unsigned char * data = ResMan::Instance()->readFile("MENTAT:FARTR.WSA", &len);

    SDL_Palette* palette = Application::Instance()->Screen()->getSurface()->format->palette;

    WsafilePtr m_wsa (new Wsafile(data, len));

    ImagePtr tmp (m_wsa->getPicture(1, palette));
    m_surf.reset(new Image(UPoint(258, 65)));
    Rect src (6,31, 82, 65);
    m_surf->blitFrom(tmp.get(), src, UPoint(0, 0));

    data = ResMan::Instance()->readFile("MENTAT:FHARK.WSA", &len);
    m_wsa.reset(new Wsafile(data, len));
    tmp.reset(m_wsa->getPicture(1, palette));
    m_surf->blitFrom(tmp.get(), src, UPoint(88, 0));

    data = ResMan::Instance()->readFile("MENTAT:FORDOS.WSA", &len);
    m_wsa.reset(new Wsafile(data, len));
    tmp.reset(m_wsa->getPicture(1, palette));
    m_surf->blitFrom(tmp.get(), src, UPoint(176, 0));

    m_surf = m_surf->getResized(2);

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

void MainMenuState::doOptions()
{
    mp_parent->PushState( new OptionsMenuState() );
}

void MainMenuState::doSkirmish()
{
    printf("skirmish!\n");
    SoundPlayer::Instance()->playMusic("SOUND:DUNE5.ADL", 6);
    
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
    m_surf.get()->blitToScreen(SPoint(Settings::Instance()->GetWidth() / 2 - m_surf->getSurface()->w/2, 
                        Settings::Instance()->GetHeight() / 4));

    return 0;
}
