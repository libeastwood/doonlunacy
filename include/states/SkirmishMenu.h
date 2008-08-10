#ifndef DUNE_STATES_SKIRMISHMENU_H
#define DUNE_STATES_SKIRMISHMENU_H

#include "states/MainMenuBase.h"

class BoringButton;
class SkirmishMenuState : public MainMenuBaseState 
{
    public:
        SkirmishMenuState();

        void doCancel();
        void doStart();

        virtual const char* GetName() { return "SkirmishMenuState"; }

    protected:
		virtual void drawSpecifics();

    private:
        BoringButton *m_butStart;
        BoringButton *m_butCancel;
};

#endif // DUNE_STATES_SINGLEMENU_H
