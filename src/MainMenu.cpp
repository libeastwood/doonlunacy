#include "MainMenu.h"

#include "Application.h"
#include "DataFile.h"
#include "Settings.h"

#include "SingleMenu.h"

#include "boost/bind.hpp"


MainMenuState::MainMenuState()
{
    //m_menuBackground = (SDL_Surface*)(dataFile[UI_Menu].dat); 
    //m_menu = new Window();
    //m_menu->setHeight(m_menuBackground->h);
    //m_menu->setWidth(m_menuBackground->w);

    m_vbox = new VBox();

    const int bw = 200;
    const int bh = 20;
    
    m_butSingle = new BoringButton("Single Player");
    m_butSingle->setSize(bw, bh);
    //m_butSingle = new GraphicButton((SDL_Surface*)(dataFile[UI_Single].dat),
    //                   (SDL_Surface*)(dataFile[UI_Single_Pressed].dat));
    m_butSingle->onClick.connect(
            boost::bind(&MainMenuState::doSingle, this) );
   
    m_vbox->addChild(m_butSingle);

    m_butMulti = new BoringButton("Multi Player");
    m_butMulti->setSize(bw, bh);
    //m_butMulti = new GraphicButton((SDL_Surface*)(dataFile[UI_Multi].dat),
    //                   (SDL_Surface*)(dataFile[UI_Multi_Pressed].dat));
    m_butMulti->onClick.connect(
            boost::bind(&MainMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butMulti);

    m_butMapEd = new BoringButton("Map Editor");
    m_butMapEd->setSize(bw, bh);
    //m_butMapEd = new GraphicButton((SDL_Surface*)(dataFile[UI_MapEdit].dat),
    //                   (SDL_Surface*)(dataFile[UI_MapEdit_Pressed].dat));
    m_butMapEd->onClick.connect(
            boost::bind(&MainMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butMapEd);

    m_butOptions = new BoringButton("Options");
    m_butOptions->setSize(bw, bh);
    //m_butOptions = new GraphicButton((SDL_Surface*)(dataFile[UI_OptionsMM].dat),
    //                   (SDL_Surface*)(dataFile[UI_OptionsMM_Pressed].dat));
    m_butOptions->onClick.connect(
            boost::bind(&MainMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butOptions);

    m_butAbout = new BoringButton("About");
    m_butAbout->setSize(bw, bh);
    //m_butAbout = new GraphicButton((SDL_Surface*)(dataFile[UI_About].dat),
    //                   (SDL_Surface*)(dataFile[UI_About_Pressed].dat));
    m_butAbout->onClick.connect(
            boost::bind(&MainMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butAbout);

    m_butQuit = new BoringButton("Quit");
    m_butQuit->setSize(bw, bh);
    //m_butQuit = new GraphicButton((SDL_Surface*)(dataFile[UI_Quit].dat),
    //                   (SDL_Surface*)(dataFile[UI_Quit_Pressed].dat));
    m_butQuit->onClick.connect(
            boost::bind(&MainMenuState::doQuit, this) );
   
    m_vbox->addChild(m_butQuit);

    m_vbox->fit(2);
    Uint16 x = (Settings::Instance()->GetWidth() / 2) - 
                (m_vbox->getWidth() / 2);
    m_vbox->setPos(x - 5, 312);
    m_vbox->reshape();
    
    m_container->addChild(m_vbox);
};

MainMenuState::~MainMenuState()
{
    delete m_butSingle;
    delete m_butMulti;
    delete m_butMapEd;
    delete m_butOptions;
    delete m_butAbout;
    delete m_butQuit;

    delete m_vbox;
};

void MainMenuState::doSkirmish()
{
    printf("skirmish!\n");
};

void MainMenuState::doSingle()
{
    mp_parent->PushState( new SingleMenuState() );
};

void MainMenuState::doQuit()
{
    PopState();
};

/*
void MainMenuState::JustMadeActive()
{
    Application::Instance()->RootWidget()->addChild(m_vbox);
    State::JustMadeActive();
};

void MainMenuState::JustMadeInactive()
{
    Application::Instance()->RootWidget()->deleteChild(m_vbox);
    State::JustMadeInactive();
};


int MainMenuState::Execute(float dt)
{
    Application::Instance()->BlitCentered(m_menuBackground);

    return 0;
};
*/
