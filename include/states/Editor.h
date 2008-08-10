#ifndef DUNE_STATES_EDITOR_H
#define DUNE_STATES_EDITOR_H

#include "GameMenu.h"

#include "gui2/MapWidget.h"

class EditorState : public MenuBaseState {
public:
	EditorState();
	~EditorState();

	int Execute(float dt);
	virtual const char* GetName() { return "Editor"; }


private:
    MapWidget *m_mapWidget, *m_mapWidget2;
};

#endif // DUNE_STATES_EDITOR_H
