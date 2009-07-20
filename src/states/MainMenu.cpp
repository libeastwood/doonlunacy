#include "states/MainMenu.h"

#include "SoundPlayer.h"
#include "Strings.h"

#include "gui2/Button.h"
#include "gui2/Frame.h"
#include "gui2/VBox.h"
#include "gui2/HScale.h"

#include "states/Editor.h"
#include "states/SingleMenu.h"
#include "states/OptionsMenu.h"

#include <boost/bind.hpp>
#include <boost/format.hpp>


MainMenuState::MainMenuState() : MainMenuBaseState()
{
    m_butSingle = new BoringButton("Single Player");
    m_butSingle->setSize(SPoint(bw, bh));
    m_butSingle->onClick.connect(
            boost::bind(&MainMenuState::doSingle, this) );
   
    m_vbox->addChild(m_butSingle);

    m_butMulti = new BoringButton("Multi Player");
    m_butMulti->setSize(SPoint(bw, bh));
    m_butMulti->onClick.connect(
            boost::bind(&MainMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butMulti);

    m_butEditor = new BoringButton("Editor");
    m_butEditor->setSize(SPoint(bw, bh));
    m_butEditor->onClick.connect(
            boost::bind(&MainMenuState::doEditor, this) );
   
    m_vbox->addChild(m_butEditor);

    m_butOptions = new BoringButton("Options");
    m_butOptions->setSize(SPoint(bw, bh));
    m_butOptions->onClick.connect(
            boost::bind(&MainMenuState::doOptions, this) );
   
    m_vbox->addChild(m_butOptions);

    m_butAbout = new BoringButton("About");
    m_butAbout->setSize(SPoint(bw, bh));
    m_butAbout->onClick.connect(
            boost::bind(&MainMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butAbout);

    m_butQuit = new BoringButton("Quit");
    m_butQuit->setSize(SPoint(bw, bh));
    m_butQuit->onClick.connect(
            boost::bind(&MainMenuState::doQuit, this) );
    
    m_vbox->addChild(m_butQuit);

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
    Font* font = FontManager::Instance()->getFont("DUNE:NEW8P.FNT");
    
    Rect rect(0,0,10, 10);
    
    Image * img = new Image(UPoint(48, 2000));
    uint16_t textw, texth;
    for (uint32_t j=0; j< 200; j++)
    {
        String index = toString(j);
        font->extents(index, textw, texth);
        font->render(index, img->getSurface(), 20, j * 10, j);
        rect.y = j * 10;
        img->fillRect(j, rect);
    }
    SDL_SaveBMP(img->getSurface(), "palette.bmp");

#endif 

#if 0
    Font* font = FontManager::Instance()->getFont("DUNE:NEW10P.FNT");
    
    Rect rect(0,0,10, 10);
    
    Image * img = new Image(UPoint(100, 2000));
    uint16_t textw, texth;
    // For intro font only use range 0..125
    for (uint32_t j=0; j< 125; j++)
    {
        String tmp; tmp+= j;
        String index = toString(j);
        font->render(tmp, img->getSurface(), 20, j * 10, j);
        font->render(index, img->getSurface(), 40, j * 10, j);
        rect.y = j * 10;
        img->fillRect(j, rect);
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

