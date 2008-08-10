#ifndef DUNE_STATES_EDITOR_H
#define DUNE_STATES_EDITOR_H

#include "states/GameMenu.h"

#include "gui2/MapWidget.h"

class EditorState : public GameMenuState {
public:
	EditorState();
	~EditorState();

	virtual const char* GetName() { return "Editor"; }

};

#endif // DUNE_STATES_EDITOR_H
