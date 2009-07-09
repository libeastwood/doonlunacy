#ifndef DUNE_STATES_GAMEMENU_H
#define DUNE_STATES_GAMEMENU_H

#include "states/MenuBase.h"

#include "houses.h"

#include "gui2/MapWidget.h"

class Frame;
class GraphicButton;
class GraphicsLabel;
class GameMenuState : public MenuBaseState {
    public:
        GameMenuState(HOUSETYPE house);
        ~GameMenuState();

        virtual int Execute(float dt);

    protected:
	virtual void resize();
	virtual void draw();
	void doMentat();

	MapWidget *m_mapWidget,
		  *m_mapWidget2;
	Frame *m_topFrame,
	      *m_sideBarFrame,
	      *m_radarFrame;
	GraphicButton *m_mentatButton,
		      *m_optionsButton;
	GraphicsLabel *m_creditsLabel,
		      *m_creditsCounterLabel,
		      *m_messageLabel;
	HOUSETYPE m_house;
};

#endif // DUNE_STATES_GAMEMENU_H

