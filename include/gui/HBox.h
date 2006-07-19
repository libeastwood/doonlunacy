#ifndef DUNE_GUI_HBOX_H
#define DUNE_GUI_HBOX_H

#include "gui/Box.h"

class HBox : public Box
{
public:
	HBox();
	~HBox();

	void resizeChildren();
private:
};

#endif // DUNE_GUI_HBOX_H
