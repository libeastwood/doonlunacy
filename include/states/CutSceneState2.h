#ifndef DUNE_STATES_CUTSCENESTATE_H
#define DUNE_STATES_CUTSCENESTATE_H

#include "states/MenuBase.h"

#include "Gfx.h"

#include <string>

#include <libconfig.h++>


using namespace libconfig;

typedef std::pair<int, std::string> textString;
typedef struct {
    int loopAt,
		rewindTo,
		loops,
		wait;
} loop;

class Animation;
class AnimationLabel;
class Frame;
class Font;
class TransparentLabel;
class vector;
class CutSceneState : public MenuBaseState 
{
   
    public:
        CutSceneState(std::string path);
        ~CutSceneState();

        virtual const char* GetName() { return "CutSceneState"; }
		int Execute(float ft);
	
	private:
		void loadScene(uint32_t scene);
		Animation *m_anim;
		AnimationLabel *m_animLabel;
		Config *m_dataConfig;
		Frame *m_sceneFrame,
			  *m_animFrame,
			  *m_textFrame;
		Font *m_font;
		ImagePtr m_lastFrame;
		SPoint m_textPosition,
			   m_animPosition;
		loop *m_loop;
		uint32_t m_curAnimFrame,
				 m_curAnimFrameTotal,
				 m_curAnimFrameStartTime,
				 m_numAnimFrames,
				 m_totalAnimFrames,
				 m_animFrameDurationTime,
				 m_curScene,
				 m_hold,
				 m_textColor;
		std::vector<ImagePtr> m_animCache;
		std::vector<textString> m_textStrings,
								m_soundStrings;
		std::string m_scene;

};

#endif // DUNE_STATES_CUTSCENESTATE_H
