#include "boost/bind.hpp"

#include "Application.h"
#include "Gfx.h"
#include "OptionsMenu.h"
#include "ResMan.h"
#include "Settings.h"


OptionsMenuState::OptionsMenuState()
{
    const int bw = 200;
    const int bh = 24;
    
    std::string m_caption;
    
    Settings* set = Settings::Instance();

    m_vbox = new VBox();
    
    set->GetFullScreen() ? m_caption = "Fullscreen" : m_caption = "Window mode";
        
    m_butWindowMode = new BoringButton(m_caption);
    m_butWindowMode->setSize(UPoint(bw, bh));
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
    m_butResolution->setSize(UPoint(bw,bh));
    m_butResolution->onClick.connect(
            boost::bind(&OptionsMenuState::doResolution, this) );
   
    m_vbox->addChild(m_butResolution);

    set->GetSound() ? m_caption = "Sound On" : m_caption = "Sound Off";
    m_butSound = new BoringButton(m_caption);
    m_butSound->setSize(UPoint(bw, bh));
    m_butSound->onClick.connect(
            boost::bind(&OptionsMenuState::doSound, this) );
            
    m_vbox->addChild(m_butSound);            
    
    set->GetMusic() ? m_caption = "Music On" : m_caption = "Music Off";
    m_butMusic = new BoringButton(m_caption);
    m_butMusic->setSize(UPoint(bw, bh));
    m_butMusic->onClick.connect(
            boost::bind(&OptionsMenuState::doMusic, this) );
            
    m_vbox->addChild(m_butMusic); 

    m_butOk = new BoringButton("Ok");
    m_butOk->setSize(UPoint(bw, bh));
    m_butOk->onClick.connect(
            boost::bind(&OptionsMenuState::doOk, this) );
   
    m_vbox->addChild(m_butOk);

	m_vbox->fit(2);
    Uint16 x = (set->GetWidth() / 2) - 
                (m_vbox->w / 2);
    m_vbox->setPosition(UPoint(x - 5, 312));
    m_vbox->reshape();
    m_container->addChild(m_vbox);

}

OptionsMenuState::~OptionsMenuState()
{
    delete m_butWindowMode;
    delete m_butResolution;
    delete m_butOk;

    delete m_vbox;
}

//FIXME: doSound, doMusic,doScreenMode and future functions with bool value could be
//handled using one function e.g. toggleSetting(variable_name);
void OptionsMenuState::doSound()
{
    Settings * set = Settings::Instance();
    set->ToggleSound();
    std::string caption;
    set->GetSound() ? caption = "Sound On" : caption = "Sound Off";
    m_butSound->setCaption(caption);    
}

void OptionsMenuState::doMusic()
{
    Settings * set = Settings::Instance();
    set->ToggleMusic();
    std::string caption;
    set->GetMusic() ? caption = "Music On" : caption = "Music Off";
    m_butMusic->setCaption(caption);    
}

void OptionsMenuState::doOptions()
{
    printf("Options");
}

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

    newSetting ? m_butWindowMode->setCaption("Fullscreen") :
        m_butWindowMode->setCaption("Window mode");
}
