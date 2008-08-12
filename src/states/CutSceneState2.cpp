#include "states/CutSceneState2.h"

#include "DataCache.h"
#include "ResMan.h"

#include "gui2/Button.h"
#include "gui2/Frame.h"
#include "gui2/Label.h"

#include <eastwood/Animation.h>
#include <eastwood/WsaFile.h>

#include <iostream>
#include <libconfig.h++>

#include <boost/bind.hpp>


using namespace libconfig;

// ------------------------------------------------------------------
// CutSceneState

CutSceneState::CutSceneState(std::string scene)
{	
	ImagePtr background(new Image(UPoint(set->GetWidth(), set->GetHeight())));
	m_backgroundFrame->changeBackground(background);
	m_sceneFrame = NULL;
    m_dataConfig = new Config();

    try
    {
		m_dataConfig->readFile("cutscenes.cfg");
    }
    catch(ParseException& ex)
    {
        LOG_FATAL("CutSceneState", "Fatal error loading configuration file on line %d: %s", 
            ex.getLine(), ex.getError());

        exit(EXIT_FAILURE);
    }
	m_scene = scene;
	m_curScene = 0;
	m_drawMenu = true;
}

CutSceneState::~CutSceneState()
{
}

void CutSceneState::loadScene(uint32_t scene)
{
	if(m_sceneFrame != NULL)
	{
		m_backgroundFrame->deleteChild(m_sceneFrame);
		delete m_sceneFrame;
	}
	m_sceneFrame = new Frame(ImagePtr(new Image(UPoint(set->GetWidth(), set->GetHeight()))));
	m_backgroundFrame->addChild(m_sceneFrame);
    std::string filename = "", palettefile = "INTRO:INTRO.PAL";
	m_curAnimFrame = 0;
	m_curAnimFrameTotal = 0;
	m_hold = 0;

	float fps;
    
    Setting &node = m_dataConfig->lookup(m_scene);

    try
    {
		node[scene].lookupValue("filename", filename);
		node[scene].lookupValue("palette", palettefile);
		node[scene].lookupValue("hold", m_hold);
		if (node[scene].exists("text"))
		{
			for (int i = node[scene]["text"].getLength()-1; i >= 0; i--)
			{
				int time = node[scene]["text"][i][0];
				std::string text;
				if(node[scene]["text"][i][1].getType() == Setting::TypeInt)
					text = DataCache::Instance()->getIntroString((int)node[scene]["text"][i][1]);
				else
					text = (const char*)node[scene]["text"][i][1];
				m_textStrings.push_back(textString(time, text));
			}
		}

		if (node[scene].exists("text_position"))
		{
			m_textPosition = SPoint((int)node[scene]["text_position"][0], (int)node[scene]["text_position"][1]);
		}
		else
		{
			m_textPosition = SPoint(0,70);
		}

		if(filename == "")
		{
			ImagePtr surface(new Image(UPoint(1,1)));
			m_animCache.push_back(surface);
			m_numAnimFrames = 1;
			m_animFrameDurationTime = 1;
			m_sceneFrame->changeBackground(surface);

		}
		else
		{
			int len;
			uint8_t *data = ResMan::Instance()->readFile(filename, &len);
			
			WsaFile *wsafile(new WsaFile(data, len, DataCache::Instance()->getPalette(palettefile)));
			m_anim = wsafile->getAnimation(0,wsafile->getNumFrames() - 1, false);
			if(node[scene].lookupValue("fps", fps))
				m_anim->setFrameRate(fps);
			m_numAnimFrames = m_anim->getNumFrames();
			m_animFrameDurationTime = m_anim->getFrameDurationTime();
		}
		
		m_totalAnimFrames = m_numAnimFrames + m_hold; //endWait + loopAnimFrames;

		m_curAnimFrameStartTime = SDL_GetTicks();
    }  
    catch(ParseException& ex)
    {
        LOG_FATAL("CutSceneState", "Setting not found %d: %s", 
            ex.getLine(), ex.getError());
    }

}
int CutSceneState::Execute(float ft)
{
	if(m_drawMenu)
	{
		loadScene(m_curScene);
		m_drawMenu = false;
	}

	if(m_animCache.empty() || m_animCache.size() <= m_curAnimFrame)
	{
		//FIXME: if using ImagePtr here and resizing it, it'll crash, why?
		Image *surface(new Image(m_anim->getFrame()));
		m_animCache.push_back(surface->getResized());
	}

	if(!m_textStrings.empty() && (uint32_t)m_textStrings.back().first == m_curAnimFrameTotal)
	{
		TransparentLabel *text = new TransparentLabel(m_textStrings.back().second);
		text->setResize(true);
		text->redraw();
		std::cout << (m_backgroundFrame->getPictureSize() /2) - text->getSize()/2 + m_textPosition.getScaled() << std::endl;
		text->setPosition((m_backgroundFrame->getPictureSize() /2) - text->getSize()/2 + m_textPosition.getScaled());
		m_sceneFrame->addChild(text);
		m_textStrings.pop_back();
	}
	if((SDL_GetTicks() - m_curAnimFrameStartTime) > m_animFrameDurationTime) {
		if(m_curAnimFrame < m_numAnimFrames - 1){
			m_sceneFrame->changeBackground(m_animCache[m_curAnimFrame]);
			m_curAnimFrame++;
		}
		m_curAnimFrameStartTime = SDL_GetTicks();
		m_curAnimFrameTotal++;

		if(m_curAnimFrameTotal >= m_totalAnimFrames)
		{
			m_animCache.clear();
			m_curScene++;
			m_drawMenu = true;
		}
	}
	return 0;
}
