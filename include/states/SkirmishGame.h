#ifndef DUNE_STATES_SKIRMISHGAME_H
#define DUNE_STATES_SKIRMISHGAME_H

#include "states/GameMenu.h"

#include "gui2/MapWidget.h"

class SkirmishGameState : public GameMenuState {
    public:
	SkirmishGameState(HOUSETYPE house, int level);

	virtual const char* GetName() { return "SkirmishGameState"; }

    protected:
	virtual void resize();
	virtual void draw();

	int m_level;
};

#endif // DUNE_STATES_SKIRMISHGAME_H
