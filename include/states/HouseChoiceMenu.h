#ifndef DUNE_STATES_HOUSECHOICEMENU_H
#define DUNE_STATES_HOUSECHOICEMENU_H

#include "MenuBase.h"

#include "gui2/Button.h"

class HouseChoiceMenuState : public MenuBaseState {
public:
	HouseChoiceMenuState();
	~HouseChoiceMenuState();

	virtual const char* GetName() { return "HouseChoiceMenuState"; }


private:
	void doAtreides();
	void doOrdos();
	void doHarkonnen();
	void doBack();

	TransparentButton* m_butAtreides;
	TransparentButton* m_butOrdos;
	TransparentButton* m_butHarkonnen;
    BoringButton* m_butBack;
};

#endif // DUNE_STATES_HOUSECHOICEMENU_H
