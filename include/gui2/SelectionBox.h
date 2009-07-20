#ifndef DUNE_GUI_SELECTIONBOX_H
#define DUNE_GUI_SELECTIONBOX_H

#include <vector>
#include "Container.h"

class GraphicButton;

class SelectionBox : public Container
{
    public:
	SelectionBox(bool multi = false);
	typedef std::list<GraphicButton*> ButtonList;

        virtual bool handleButtonDown(uint8_t button, SPoint p);
        virtual bool handleButtonUp(uint8_t button, SPoint p) { return false; }

	void addChild(GraphicButton *child);

    private:
	bool m_multi;
	ButtonList m_buttons;

};

#endif // DUNE_GUI_VBOX_H
