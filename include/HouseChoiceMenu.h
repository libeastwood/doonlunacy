#ifndef DUNE_HOUSECHOICEMENU_H
#define DUNE_HOUSECHOICEMENU_H

#include "MenuBase.h"

#include "gui2/Button.h"

class HouseChoiceMenuState : public MenuBaseState {
public:
	HouseChoiceMenuState();
	~HouseChoiceMenuState();

	int Execute(float dt);
	virtual const char* GetName() { return "HouseChoiceMenuState"; }


private:
	void doAtreides();
	void doOrdos();
	void doHarkonnen();
	void doBack();

	TranspButton* m_butAtreides;
	TranspButton* m_butOrdos;
	TranspButton* m_butHarkonnen;
    BoringButton* m_butBack;
};

#endif // DUNE_HOUSECHOICEMENU_H
