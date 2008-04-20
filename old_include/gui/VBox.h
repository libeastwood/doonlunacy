#ifndef DUNE_GUI_VBOX_H
#define DUNE_GUI_VBOX_H

#include "gui/Box.h"

class VBox : public Box
{
public:
	VBox();
	~VBox();

	void resizeChildren();
private:
};

#endif // DUNE_GUI_VBOX_H
