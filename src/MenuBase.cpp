#include "MainMenu.h"

#include "Application.h"

#include <boost/bind.hpp>


MenuBaseState::MenuBaseState()
{
    m_drawMenu = true;
    m_menuBackground.reset(new Image(UPoint(set->GetWidth(), set->GetHeight())));


    m_container = new Container();
    m_container->setPosition(UPoint(0, 0));
    m_container->setSize(UPoint(set->GetWidth(),
			    set->GetHeight()));
    
    Application::Instance()->RootWidget()->addChild(m_container);
}

MenuBaseState::~MenuBaseState()
{
    delete m_container;
}

void MenuBaseState::JustMadeActive()
{
    Application::Instance()->RootWidget()->addChild(m_container);
    State::JustMadeActive();
}

void MenuBaseState::JustMadeInactive()
{
    Application::Instance()->RootWidget()->deleteChild(m_container);
    State::JustMadeInactive();
}

int MenuBaseState::Execute(float dt)
{
    return 0;
}
