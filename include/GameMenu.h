#ifndef GAMEMENU_H
#define GAMEMENU_H

#include "MenuBase.h"

#include "gui2/MapWidget.h"

class GameMenuState : public MenuBaseState {
    public:
        GameMenuState();
        ~GameMenuState();

        int Execute(float dt);

    protected:
        MapWidget *m_mapWidget, *m_mapWidget2;
	void drawMenu();
};

#endif // GAMEMENU_H

