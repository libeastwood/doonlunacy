#include "states/OptionsMenu.h"

#include "Application.h"

#include "gui2/Button.h"
#include "gui2/HScale.h"
#include "gui2/VBox.h"

#include <boost/bind.hpp>

OptionsMenuState::OptionsMenuState() : MainMenuBaseState()
{
    std::string m_caption;
    

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

    set->GetPlayIntro() ? m_caption = "Intro On" : m_caption = "Intro Off";
    m_butIntro = new BoringButton(m_caption);
    m_butIntro->setSize(UPoint(bw, bh));
    m_butIntro->onClick.connect(
            boost::bind(&OptionsMenuState::doIntro, this) );
            
    m_vbox->addChild(m_butIntro); 

    /* Disabled for now as we don't currently don't ship several opl emulators..
    m_caption = "OPL Emulator ";
    char num[2];
    snprintf(num, 2, "%d", set->GetEmuOpl());
    m_caption += num;
    m_butEmuOpl = new BoringButton(m_caption);
    m_butEmuOpl->setSize(UPoint(bw, bh));
    m_butEmuOpl->onClick.connect(
            boost::bind(&OptionsMenuState::doEmuOpl, this) );
   
    m_vbox->addChild(m_butEmuOpl);
*/
    m_butOk = new BoringButton("Ok");
    m_butOk->setSize(UPoint(bw, bh));
    m_butOk->onClick.connect(
            boost::bind(&OptionsMenuState::doOk, this) );
   
    m_vbox->addChild(m_butOk);

    m_hscaleGameSpeed = new HScale(1, 10, set->GetGameSpeed());
    m_hscaleGameSpeed->setSize(SPoint(bw, bh));
    m_vbox->addChild(m_hscaleGameSpeed);

    m_vbox->fit(2);
    Uint16 x = (set->GetWidth() / 2) - 
                (m_vbox->w / 2);
    m_vbox->setPosition(UPoint(x - 5, 312));
    m_vbox->reshape();
}

//FIXME: doSound, doMusic,doScreenMode and future functions with bool value could be
//handled using one function e.g. toggleSetting(variable_name);
void OptionsMenuState::doSound()
{
    set->ToggleSound();
    std::string caption;
    set->GetSound() ? caption = "Sound On" : caption = "Sound Off";
    m_butSound->setCaption(caption);    
}

void OptionsMenuState::doMusic()
{
    set->ToggleMusic();
    std::string caption;
    set->GetMusic() ? caption = "Music On" : caption = "Music Off";
    m_butMusic->setCaption(caption);    
}

void OptionsMenuState::doIntro()
{
    set->ToggleIntro();
    std::string caption;
    set->GetPlayIntro() ? caption = "Intro On" : caption = "Intro Off";
    m_butIntro->setCaption(caption);    
}

void OptionsMenuState::doEmuOpl()
{
	std::string caption = "OPL Emulator ";
	char num[2];
	snprintf(num, 2, "%d", Settings::Instance()->ToggleEmuOpl());
	caption += num;
	m_butEmuOpl->setCaption(caption);
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
    switch (set->ToggleResolution())
    {
        case 640:
            m_butResolution->setCaption("640x480");
            break;                

        case 800:
            m_butResolution->setCaption("800x600");
            break;
                
        case 1024:
            m_butResolution->setCaption("1024x768");
            break;
                
    }
}

void OptionsMenuState::doScreenMode()
{
    bool newSetting = set->ToggleFullscreen();

    newSetting ? m_butWindowMode->setCaption("Fullscreen") :
    m_butWindowMode->setCaption("Window mode");
}
