#ifndef DUNE_STATES_SKIRMISHMENU_H
#define DUNE_STATES_SKIRMISHMENU_H

#include "states/MainMenuBase.h"

class BoringButton;
class Counter;
class Frame;
class SelectionBox;
class TransparentLabel;

class SkirmishMenuState : public MainMenuBaseState 
{
    public:
        SkirmishMenuState();

        void doCancel();
        void doStart();

        virtual const char* GetName() { return "SkirmishMenuState"; }

    protected:
	virtual void drawSpecifics();
	virtual void resize();

    private:
	void setHouse(HOUSETYPE house) { m_house = house; }
        BoringButton *m_butStart,
		     *m_butCancel,
		     *m_butMinus,
		     *m_butPlus;
	Counter *m_missionCounter;
	Frame *m_missionFrame;
	SelectionBox *m_selectionBox;
	TransparentLabel *m_missionLabel;
	HOUSETYPE m_house;
	int m_level;

};

#endif // DUNE_STATES_SINGLEMENU_H
