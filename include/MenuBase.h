#ifndef DUNE_MENUBASE_H
#define DUNE_MENUBASE_H

#include "State.h"

class Container;
class MenuBaseState : public State 
{
    public:
        MenuBaseState();
        ~MenuBaseState();

        void JustMadeActive();
        void JustMadeInactive();

        int Execute(float dt);

    protected:
        ImagePtr m_menuBackground;
        Container* m_container;
	bool m_drawMenu;
};

#endif // DUNE_MENUBASE_H
