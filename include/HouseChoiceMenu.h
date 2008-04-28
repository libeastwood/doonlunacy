#ifndef HOUSECHOICEMENU_H_INCLUDED
#define HOUSECHOICEMENU_H_INCLUDED

#include "MenuBase.h"

class TranspButton;
class VBox;

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
	VBox* m_vbox;	

	TranspButton* m_butAtreides;
	TranspButton* m_butOrdos;
	TranspButton* m_butHarkonnen;
};

#endif // HOUSECHOICEMENU_H_INCLUDED
