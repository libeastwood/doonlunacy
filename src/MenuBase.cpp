#include "MainMenu.h"

#include "Application.h"
#include "DataFile.h"
#include "Settings.h"

#include "SingleMenu.h"

#include "boost/bind.hpp"

#include <assert.h>


MenuBaseState::MenuBaseState()
{
    m_menuBackground = (SDL_Surface*)(dataFile[UI_Menu].dat); 
    //m_menu = new Window();
    //m_menu->setHeight(m_menuBackground->h);
    //m_menu->setWidth(m_menuBackground->w);

    m_container = new Container();
    m_container->setPos(0, 0);
    m_container->setSize(Settings::Instance()->GetWidth(),
                         Settings::Instance()->GetHeight());
    
    Application::Instance()->RootWidget()->addChild(m_container);
};

MenuBaseState::~MenuBaseState()
{
    delete m_container;
};

void MenuBaseState::JustMadeActive()
{
    Application::Instance()->RootWidget()->addChild(m_container);
    State::JustMadeActive();
};

void MenuBaseState::JustMadeInactive()
{
    Application::Instance()->RootWidget()->deleteChild(m_container);
    State::JustMadeInactive();
};

int MenuBaseState::Execute(float dt)
{
    //Application::Instance()->BlitCentered(m_menuBackground);

    return 0;
};
