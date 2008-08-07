#ifndef MENTAITNFOMENU_H_INCLUDED
#define MENTATINFOMENU_H_INCLUDED

#include "MentatMenu.h"
class GraphicButton;
class AnimationLabel;
class MentatInfoMenuState : public MentatMenuState {
public:
	MentatInfoMenuState(HOUSETYPE newHouse);
	
	virtual const char* GetName() { return "MentatInfoMenuState"; }
		
private:
        GraphicButton* m_butExit;
	AnimationLabel* m_planetAnimation;
	void doExit();
};

#endif // MENTATINFOMENU_H_INCLUDED

