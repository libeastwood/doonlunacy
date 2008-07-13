#ifndef EDITOR_H
#define EDITOR_H

#include "GameMenu.h"

class EditorState : public GameMenuState {
public:
	EditorState();
	~EditorState();

	virtual const char* GetName() { return "Editor"; }
};

#endif // EDITOR_H
