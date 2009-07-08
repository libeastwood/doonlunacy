#include <boost/bind.hpp>

#include "states/MainMenu.h"

#include "Application.h"
#include "Settings.h"
#include "gui2/Container.h"
#include "gui2/Frame.h"

MenuBaseState::MenuBaseState()
{
    m_drawMenu = true;

    m_container = new Container();
    m_container->setPosition(UPoint(0, 0));
    m_container->setSize(UPoint(set->GetWidth(),
			    set->GetHeight()));
    
    m_backgroundFrame = new Frame();
    m_container->addChild(m_backgroundFrame);
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
