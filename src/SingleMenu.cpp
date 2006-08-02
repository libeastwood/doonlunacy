#include "SingleMenu.h"

#include "Application.h"
#include "DataFile.h"
#include "Settings.h"

#include "boost/bind.hpp"


SingleMenuState::SingleMenuState()
{
    //m_menuBackground = (SDL_Surface*)(dataFile[UI_Menu].dat); 
    //m_menu = new Window();
    //m_menu->setHeight(m_menuBackground->h);
    //m_menu->setWidth(m_menuBackground->w);

    const int bw = 200;
    const int bh = 20;

    m_vbox = new VBox();
    
    m_butCampaign = new BoringButton("Campaign");
    m_butCampaign->setSize(bw, bh);
    m_butCampaign->onClick.connect(
            boost::bind(&SingleMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butCampaign);

    m_butCustom = new BoringButton("Custom Game");
    m_butCustom->setSize(bw,bh);
    m_butCustom->onClick.connect(
            boost::bind(&SingleMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butCustom);

    m_butSkirmish = new BoringButton("Skirmish");
    m_butSkirmish->setSize(bw, bh);
    m_butSkirmish->onClick.connect(
            boost::bind(&SingleMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butSkirmish);

    m_butLoad = new BoringButton("Load Game");
    m_butLoad->setSize(bw, bh);
    m_butLoad->onClick.connect(
            boost::bind(&SingleMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butLoad);

    m_butCancel = new BoringButton("Cancel");
    m_butCancel->setSize(bw, bh);
    m_butCancel->onClick.connect(
            boost::bind(&SingleMenuState::doCancel, this) );
   
    m_vbox->addChild(m_butCancel);

    m_vbox->fit(2);
    Uint16 x = (Settings::Instance()->GetWidth() / 2) - 
                (m_vbox->getWidth() / 2);
    m_vbox->setPos(x - 5, 312);
    m_vbox->reshape();
};

SingleMenuState::~SingleMenuState()
{
    delete m_butCampaign;
    delete m_butCustom;
    delete m_butSkirmish;
    delete m_butLoad;
    delete m_butCancel;

    delete m_vbox;
};

void SingleMenuState::doSkirmish()
{
    printf("Skirmish\n");
};

void SingleMenuState::doCancel()
{
    assert(mp_parent != NULL);
    PopState();
};

void SingleMenuState::JustMadeActive()
{
    State::JustMadeActive();
    Application::Instance()->RootWidget()->addChild(m_vbox);
};

void SingleMenuState::JustMadeInactive()
{
    State::JustMadeInactive();
    Application::Instance()->RootWidget()->deleteChild(m_vbox);
};

int SingleMenuState::Execute(float dt)
{
    //Application::Instance()->BlitCentered(m_menuBackground);

    return 0;
};
