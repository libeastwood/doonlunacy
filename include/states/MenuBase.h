#ifndef DUNE_MENUBASE_H
#define DUNE_MENUBASE_H

#include "State.h"

class Container;
class Frame;
class MenuBaseState : public State 
{
    public:
	MenuBaseState();
	~MenuBaseState();

	virtual void JustMadeActive();
	virtual void JustMadeInactive();

	virtual int Execute(float dt);

    protected:
	virtual void resize();

	Container *m_container;
	Frame *m_backgroundFrame;
	bool m_drawMenu;
};

#endif // DUNE_MENUBASE_H
