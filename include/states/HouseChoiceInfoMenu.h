#ifndef DUNE_STATES_HOUSECHOICEINFOMENU_H
#define DUNE_STATES_HOUSECHOICEINFOMENU_H

#include "MentatMenu.h"

class AnimationLabel;
class GraphicButton;
class HouseChoiceInfoMenuState : public MentatMenuState {
	public:
		HouseChoiceInfoMenuState(HOUSETYPE newHouse);
		~HouseChoiceInfoMenuState();
		
		void doNo();
		
		virtual const char* GetName() { return "HouseChoiceInfoMenuState"; }
		
	private:
        GraphicButton* m_butYes;
        GraphicButton* m_butNo;
		AnimationLabel* m_planetAnimation;
};

#endif // DUNE_STATES_HOUSECHOICEINFOMENU_H

