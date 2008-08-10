#ifndef DUNE_STATES_GAMEMENU_H
#define DUNE_STATES_GAMEMENU_H

#include "states/MenuBase.h"

#include "gui2/MapWidget.h"

class GraphicButton;
class GameMenuState : public MenuBaseState {
    public:
        GameMenuState();
        ~GameMenuState();

        int Execute(float dt);

    protected:
        MapWidget *m_mapWidget, *m_mapWidget2;
		GraphicButton *m_mentatButton, *m_optionsButton;
		void drawMenu();
		void doMentat();
};

#endif // DUNE_STATES_GAMEMENU_H

