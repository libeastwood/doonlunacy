#ifndef DUNE_STATES_SKIRMISHMENU_H
#define DUNE_STATES_SKIRMISHMENU_H

#include "states/MainMenuBase.h"

class BoringButton;
class Counter;
class Frame;
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
        BoringButton *m_butStart,
		     *m_butCancel,
		     *m_butMinus,
		     *m_butPlus;
	Counter *m_missionCounter;
	Frame *m_missionFrame;
	TransparentLabel *m_missionLabel;

};

#endif // DUNE_STATES_SINGLEMENU_H
