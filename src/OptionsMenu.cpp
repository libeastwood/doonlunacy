#include "OptionsMenu.h"

#include "Application.h"
#include "DataFile.h"
#include "Settings.h"

#include "boost/bind.hpp"


OptionsMenuState::OptionsMenuState()
{
    //m_menuBackground = (SDL_Surface*)(dataFile[UI_Menu].dat); 
    //m_menu = new Window();
    //m_menu->setHeight(m_menuBackground->h);
    //m_menu->setWidth(m_menuBackground->w);

    const int bw = 200;
    const int bh = 20;
    
    std::string m_caption;
    
    Settings* set = Settings::Instance();

    m_vbox = new VBox();
    
    if (set->GetFullScreen())
        m_caption = "Fullscreen";
    else
        m_caption = "Window mode";
        
    m_butWindowMode = new BoringButton(m_caption);
    m_butWindowMode->setSize(bw, bh);
    m_butWindowMode->onClick.connect(
            boost::bind(&OptionsMenuState::doScreenMode, this) );
   
    m_vbox->addChild(m_butWindowMode);

    switch (set->GetWidth())
    {
        case 640:
            m_caption = "640x480";
            break;
                
        case 800:
            m_caption = "800x600";
            break;
                
        case 1024:
            m_caption = "1024x768";
            break;                
    
    }

    m_butResolution = new BoringButton(m_caption);
    m_butResolution->setSize(bw,bh);
    m_butResolution->onClick.connect(
            boost::bind(&OptionsMenuState::doResolution, this) );
   
    m_vbox->addChild(m_butResolution);

    m_butOk = new BoringButton("Ok");
    m_butOk->setSize(bw, bh);
    m_butOk->onClick.connect(
            boost::bind(&OptionsMenuState::doOk, this) );
   
    m_vbox->addChild(m_butOk);

    m_vbox->fit(2);
    Uint16 x = (Settings::Instance()->GetWidth() / 2) - 
                (m_vbox->getWidth() / 2);
    m_vbox->setPos(x - 5, 312);
    m_vbox->reshape();
};

OptionsMenuState::~OptionsMenuState()
{
    delete m_butWindowMode;
    delete m_butResolution;
    delete m_butOk;

    delete m_vbox;
};

void OptionsMenuState::doOptions()
{
    printf("Options");
};

void OptionsMenuState::doOk()
{
    assert(mp_parent != NULL);
    PopState();
}

void OptionsMenuState::doResolution()
{
    Application* app = Application::Instance();
    Settings* set = Settings::Instance();

    switch (set->GetWidth())
    {
        case 640:
            app->UpdateVideoMode(800, 600);
            m_butResolution->setCaption("800x600");
            break;
                
        case 800:
            app->UpdateVideoMode(1024, 768);
            m_butResolution->setCaption("1024x768");
            break;
                
        case 1024:
            app->UpdateVideoMode(640, 480);
            m_butResolution->setCaption("640x480");
            break;                
    }
}

void OptionsMenuState::doScreenMode()
{
    Settings * set = Settings::Instance();
    bool newSetting = !set->GetFullScreen();
    
    Application::Instance()->UpdateVideoMode(newSetting);

    if (newSetting)
        m_butWindowMode->setCaption("Fullscreen");
    else
        m_butWindowMode->setCaption("Window mode");
}

void OptionsMenuState::JustMadeActive()
{
    State::JustMadeActive();
    Application::Instance()->RootWidget()->addChild(m_vbox);
};

void OptionsMenuState::JustMadeInactive()
{
    State::JustMadeInactive();
    Application::Instance()->RootWidget()->deleteChild(m_vbox);
};

int OptionsMenuState::Execute(float dt)
{
    //Application::Instance()->BlitCentered(m_menuBackground);

    return 0;
};