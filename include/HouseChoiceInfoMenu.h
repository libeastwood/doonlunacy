#ifndef HOUSECHOICEINFOMENU_H_INCLUDED
#define HOUSECHOICEINFOMENU_H_INCLUDED

#include "MentatMenu.h"
class HouseChoiceInfoMenuState : public MentatMenuState {
	public:
		HouseChoiceInfoMenuState(HOUSETYPE newHouse);
		~HouseChoiceInfoMenuState();
		
		virtual const char* GetName() { return "HouseChoiceInfoMenuState"; }
		
	private:
        GraphicButton* m_butYes;
        GraphicButton* m_butNo;		
};

#endif // HOUSECHOICEINFOMENU_H_INCLUDED

