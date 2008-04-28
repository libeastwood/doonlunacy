#ifndef HOUSECHOICEINFOMENU_H_INCLUDED
#define HOUSECHOICEINFOMENU_H_INCLUDED

#include "MentatMenu.h"
class GraphicButton;
class AnimationLabel;
class HouseChoiceInfoMenuState : public MentatMenuState {
	public:
		HouseChoiceInfoMenuState(HOUSETYPE newHouse);
		~HouseChoiceInfoMenuState();
		
		virtual const char* GetName() { return "HouseChoiceInfoMenuState"; }
		
	private:
        GraphicButton* m_butYes;
        GraphicButton* m_butNo;
		AnimationLabel* m_planetAnimation;
};

#endif // HOUSECHOICEINFOMENU_H_INCLUDED

