#include "SingleMenu.h"

#include "Application.h"
#include "DataFile.h"
#include "Settings.h"

#include "boost/bind.hpp"


SingleMenuState::SingleMenuState()
{
    m_menuBackground = (SDL_Surface*)(dataFile[UI_Menu].dat); 
    //m_menu = new Window();
    //m_menu->setHeight(m_menuBackground->h);
    //m_menu->setWidth(m_menuBackground->w);

    m_vbox = new VBox();
    
    m_butCampaign = new GraphicButton((SDL_Surface*)(dataFile[UI_Single_Campaign].dat),
                       (SDL_Surface*)(dataFile[UI_Single_Campaign_Pressed].dat));
    m_butCampaign->onClick.connect(
            boost::bind(&SingleMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butCampaign);

    m_butCustom = new GraphicButton((SDL_Surface*)(dataFile[UI_Single_Custom].dat),
                       (SDL_Surface*)(dataFile[UI_Single_Custom_Pressed].dat));
    m_butCustom->onClick.connect(
            boost::bind(&SingleMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butCustom);

    m_butSkirmish = new GraphicButton((SDL_Surface*)(dataFile[UI_Single_Skirmish].dat),
                       (SDL_Surface*)(dataFile[UI_Single_Skirmish_Pressed].dat));
    m_butSkirmish->onClick.connect(
            boost::bind(&SingleMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butSkirmish);

    m_butLoad = new GraphicButton((SDL_Surface*)(dataFile[UI_Load].dat),
                       (SDL_Surface*)(dataFile[UI_Load_Pressed].dat));
    m_butLoad->onClick.connect(
            boost::bind(&SingleMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butLoad);

    m_butCancel = new GraphicButton((SDL_Surface*)(dataFile[UI_Cancel].dat),
                       (SDL_Surface*)(dataFile[UI_Cancel_Pressed].dat));
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
    printf("skirmish!\n");
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
    Application::Instance()->BlitCentered(m_menuBackground);

    return 0;
};
