#ifndef DUNE_GUI_BOX_H
#define DUNE_GUI_BOX_H

#include "gui/Container.h"

class Box : public Container
{
public:
	Box();
	~Box();

	void addChild(Widget* newChild);
	virtual void resizeChildren() = 0;
private:
};

#endif // DUNE_GUI_BOX_H
