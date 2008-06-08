#ifndef HOUSECHOICEMENU_H_INCLUDED
#define HOUSECHOICEMENU_H_INCLUDED

#include "MenuBase.h"

#include "gui2/MapWidget.h"

class EditorState : public MenuBaseState {
public:
	EditorState();
	~EditorState();

	int Execute(float dt);
	virtual const char* GetName() { return "Editor"; }


private:
    MapWidget * m_mapWidget;
};

#endif // HOUSECHOICEMENU_H_INCLUDED
