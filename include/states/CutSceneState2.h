#ifndef DUNE_STATES_CUTSCENESTATE_H
#define DUNE_STATES_CUTSCENESTATE_H

#include "states/MenuBase.h"

#include <string>

#include <libconfig.h++>


using namespace libconfig;

class Animation;
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
		Config *m_dataConfig;
		uint32_t m_curAnimFrame,
				 m_curAnimFrameTotal,
				 m_curAnimFrameStartTime,
				 m_numAnimFrames,
				 m_totalAnimFrames,
				 m_animFrameDurationTime,
				 m_curScene,
				 m_hold;
		std::vector<ImagePtr> m_animCache;
		std::string m_scene;

};

#endif // DUNE_STATES_CUTSCENESTATE_H
