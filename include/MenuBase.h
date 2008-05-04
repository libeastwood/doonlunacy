#ifndef DUNE_MENUBASE_H
#define DUNE_MENUBASE_H

#include "State.h"
#include "gui2/Container.h"

#include "SDL.h"

class MenuBaseState : public State 
{
    public:
        MenuBaseState();
        ~MenuBaseState();

        void JustMadeActive();
        void JustMadeInactive();

        int Execute(float dt);

    public:
        ImagePtr m_menuBackground;
        Container* m_container;
};

#endif // DUNE_MENUBASE_H
