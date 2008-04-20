#ifndef DUNE_GUI_CONTAINER_H
#define DUNE_GUI_CONTAINER_H

#include "gui/Widget.h"
#include "ListIterator.h"

class Container : public Widget
{
public:
	Container();
	virtual ~Container();

	virtual void addChild(Widget* newChild);
	void draw();
	void dropChildren();
	void handleCancelPress(Uint32 x, Uint32 y);
	void handleCancelRelease(Uint32 x, Uint32 y);
	void handleMotion(Uint32 x, Uint32 y);
	void handlePress(Uint32 x, Uint32 y);
	void handleRelease(Uint32 x, Uint32 y);
	void setBuffering(int newBuffering);
protected:
	Uint16 buffering;
	ListIterator children;
};

#endif // DUNE_GUI_CONTAINER_H
