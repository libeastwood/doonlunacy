#include "MainMenu.h"

#include "Application.h"
//#include "DataFile.h"
#include "Settings.h"
#include "ResMan.h"
#include "SingleMenu.h"
#include "OptionsMenu.h"

#include "boost/bind.hpp"
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

    SDL_Palette* palette = Application::Instance()->Screen()->format->palette;

    Wsafile * m_wsa = new Wsafile(data, len);

    SDL_Surface * tmp = copySurface(m_wsa->getPicture(1, palette));

    m_surf = SDL_CreateRGBSurface(SDL_SWSURFACE, 258, 65, 8,0,0,0,0);

    SDL_SetColors(m_surf, palette->colors, 0, palette->ncolors);

    Rect src (6,31, 82, 65);

    Rect cp(0, 0, 82, 65);

    SDL_BlitSurface(tmp, &src, m_surf, &cp); 


    data = ResMan::Instance()->readFile("MENTAT:FHARK.WSA", &len);

    m_wsa = new Wsafile(data, len);
    tmp = copySurface(m_wsa->getPicture(1, palette));

    cp.setPosition(SPoint(88, 0));

    SDL_BlitSurface(tmp, &src, m_surf, &cp); 

    data = ResMan::Instance()->readFile("MENTAT:FORDOS.WSA", &len);

    m_wsa = new Wsafile(data, len);
    tmp = copySurface(m_wsa->getPicture(1, palette));

    cp.setPosition(SPoint(176, 0));
    SDL_BlitSurface(tmp, &src, m_surf, &cp);

    m_surf = resizeSurface(m_surf, 2);

    m_rect.setPosition(SPoint(Settings::Instance()->GetWidth() / 2 - m_surf->w/2, 
                        Settings::Instance()->GetHeight() / 4)); 

    m_rect.setSize(UPoint(m_surf->w, m_surf->h));
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
    Application::Instance()->Blit(m_surf, NULL, &m_rect);

    return 0;
}
