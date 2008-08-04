#ifndef HOUSECHOICEMENU_H_INCLUDED
#define HOUSECHOICEMENU_H_INCLUDED

#include "MenuBase.h"

#include "gui2/Button.h"
#include "gui2/VBox.h"

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
	VBox* m_vbox;	

	TranspButton* m_butAtreides;
	TranspButton* m_butOrdos;
	TranspButton* m_butHarkonnen;
    BoringButton* m_butBack;
};

#endif // HOUSECHOICEMENU_H_INCLUDED
