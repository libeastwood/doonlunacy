#ifndef DUNE_SKIRMISHMENU_H
#define DUNE_SKIRMISHMENU_H

#include "MainMenuBase.h"

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

#endif // DUNE_SINGLEMENU_H
