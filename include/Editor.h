#ifndef EDITOR_H
#define EDITOR_H

#include "MenuBase.h"

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

#endif // EDITOR_H
