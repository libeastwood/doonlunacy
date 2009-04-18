#include <iostream>
#include <libconfig.h++>
#include <boost/bind.hpp>
#include <eastwood/CpsFile.h>
#include <eastwood/WsaFile.h>

#include "states/CutSceneState2.h"

#include "DataCache.h"
#include "FontManager.h"
#include "GameData.h"
#include "ResMan.h"
#include "SoundPlayer.h"

#include "gui2/Button.h"
#include "gui2/Frame.h"
#include "gui2/Label.h"

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
		m_dataConfig->setAutoConvert(true);
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
	m_font = FontManager::Instance()->getFont("INTRO:INTRO.FNT");
}

CutSceneState::~CutSceneState()
{
	delete m_dataConfig;
	if(m_loop)
		delete m_loop;
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
	m_animFrame = new Frame();
	m_sceneFrame->addChild(m_animFrame);
	m_textFrame = new Frame(ImagePtr(new Image(UPoint(300,50).getScaled())));
	m_sceneFrame->addChild(m_textFrame);
    std::string filename = "", palettefile = "INTRO:INTRO.PAL";
	m_hold = 0;
	m_textColor = 49;
	m_loop = NULL;
	int song,
		loopAnimFrames = 0;
	float fps = 8.0;
	bool continuation = false;
	m_fadeIn = false;
	m_fadeOut = false;
	m_textFadeIn = false;

    Setting &node = m_dataConfig->lookup(m_scene);
    try
    {
		node[scene].lookupValue("filename", filename);
		
		node[scene].lookupValue("palette", palettefile);
		node[scene].lookupValue("hold", m_hold);
		node[scene].lookupValue("text_color", m_textColor);
		node[scene].lookupValue("continuation", continuation);
		node[scene].lookupValue("fps", fps);
		node[scene].lookupValue("fade_in", m_fadeIn);
		node[scene].lookupValue("fade_out", m_fadeOut);

		if(node[scene].lookupValue("song", song))
			SoundPlayer::Instance()->playMusic(MUSIC_INTRO, song);

		if(node[scene].exists("loop"))
		{
			m_loop = new loop;
			m_loop->loopAt = node[scene]["loop"]["loopat"];
			m_loop->rewindTo = node[scene]["loop"]["rewindto"];
			m_loop->loops = node[scene]["loop"]["loops"];
			m_loop->wait = node[scene]["loop"]["wait"];
			loopAnimFrames = (m_loop->loopAt - m_loop->rewindTo) * m_loop->loops + m_loop->wait;
		}
		if (node[scene].exists("text"))
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
		if (node[scene].exists("sound"))
			for (int i = node[scene]["sound"].getLength()-1; i >= 0; i--)
			{
				int time = node[scene]["sound"][i][0];
				std::string sound = node[scene]["sound"][i][1];
				m_soundStrings.push_back(textString(time, sound));
			}

		if (node[scene].exists("text_position"))
			m_textPosition = SPoint((int)node[scene]["text_position"][0], (int)node[scene]["text_position"][1]);
		else
			m_textPosition = SPoint(0,70);
		m_textFrame->setPosition(m_textPosition.getScaled() + SPoint(0, m_backgroundFrame->getSize().y /2 - m_textFrame->getSize().y/2));

		if(node[scene].exists("anim_position"))
			m_animPosition = SPoint((int)node[scene]["anim_position"][0], (int)node[scene]["anim_position"][1]);
		else
			m_animPosition = SPoint(0,-20);

		m_animLabel = new AnimationLabel();		
		ImagePtr animFrame;
		if(filename == "")
			m_animLabel->addFrame(ImagePtr(new Image(UPoint(1,1))), true);
		else
		{
			size_t len;
			uint8_t *data = ResMan::Instance()->readFile(filename, &len);
			if(filename.substr(filename.length()-3, 3) == "CPS")
			{
				CpsFile *cpsfile(new CpsFile(data, len));
				m_animLabel->addFrame(ImagePtr(new Image(cpsfile->getSurface())), true);
				delete cpsfile;
			}
			else
			{
				WsaFile *wsafile;
				if(continuation)
					wsafile = new WsaFile(data, len, DataCache::Instance()->getPalette(palettefile),m_lastFrame->getSurface());
				else
					wsafile = new WsaFile(data, len, DataCache::Instance()->getPalette(palettefile));
				std::vector<ImagePtr> wsaFrames;
				for(uint32_t i = 0; i < wsafile->getNumFrames(); i++)
					wsaFrames.push_back(ImagePtr(new Image(wsafile->getSurface(i))));
				delete wsafile;
				
				for(uint32_t i = 0; i < wsaFrames.size() + loopAnimFrames; i++)
					if(i < wsaFrames.size())
					{
						m_animLabel->addFrame(wsaFrames[i], true);
						if(m_loop && (int)i == m_loop->loopAt && m_loop->loops > -1)
						{
							if(m_loop->wait > 0)
								m_animLabel->addPause(m_loop->wait), m_loop->wait=0;
							i = m_loop->rewindTo, m_loop->loops--;
						}
					}
				m_lastFrame = wsaFrames.back();
			}
		}
		m_animLabel->addPause(m_hold);

		m_animLabel->setFrameRate(fps);

		SPoint pos = (m_backgroundFrame->getSize() /2) - m_animLabel->getSize()/2 + m_animPosition.getScaled();
		// Don't allow picture picture to be placed outside of screen
		if(pos.x < 0) pos.x = 0;
		if(pos.y < 0) pos.y = 0;
		m_animLabel->setPosition(pos);

		m_sceneFrame->addChild(m_animLabel);

    }  
    catch(ParseException& ex)
    {
        LOG_FATAL("CutSceneState", "Setting not found %d: %s", 
            ex.getLine(), ex.getError());
    }
	m_sceneStart = SDL_GetTicks();
}
int CutSceneState::Execute(float ft)
{
	if(m_drawMenu)
	{
		loadScene(m_curScene);
		m_drawMenu = false;
	}
	if(m_animLabel->getCurFrame() == 0 && m_fadeIn)
		m_fadeIn = m_backgroundFrame->fadeInChildren();

	if(m_animLabel->getCurFrame() == m_animLabel->getNumFrames() - 1)
	{
		if(m_fadeOut)
			m_fadeOut = m_backgroundFrame->fadeOutChildren();
		else
		{
			LOG_INFO("CutSceneState", "Scene %d playtime: %f s", m_curScene, (SDL_GetTicks()- m_sceneStart)/1000.f);
			m_curScene++;
			if(m_loop != NULL)
				free(m_loop);
			if(m_curScene == (uint32_t)m_dataConfig->lookup(m_scene).getLength())
				return -1;
			m_drawMenu = true;
		}
	}

	if((!m_textStrings.empty() && (uint32_t)m_textStrings.back().first == m_animLabel->getCurFrame() + 2)
			|| (!m_fadeOut && m_animLabel->getCurFrame() == m_animLabel->getNumFrames() - 2))
		m_textFrame->fadeOut(10);

	if(!m_textStrings.empty() && (uint32_t)m_textStrings.back().first == m_animLabel->getCurFrame())
	{
			ImagePtr tmp(new Image(UPoint(420, 45)));
			tmp->setColorKey();
			std::string text = m_textStrings.back().second;
			uint8_t numLines = 0;
			int linebreak = text.find("\n",0)+ 1;
			std::string thisLine;
			// This is a bit hairy and needs to be cleaned up a bit..
			Uint16 textw, texth;

			while(text.substr(0, linebreak-1).length() > 0){
				thisLine = text.substr(0, linebreak-1);
				if(linebreak != -1)
					thisLine += " ";
				
				m_font->extents(thisLine, textw, texth);
				
				m_font->render(thisLine, tmp->getSurface(), tmp->getSize().x/2 - textw/2, 10+(numLines++*20) - texth/2, m_textColor);
				
				if(linebreak == -1 || text == text.substr(linebreak, text.length()-linebreak))
					break;
				text = text.substr(linebreak, text.length()-linebreak);
				linebreak = text.find("\n",0);
			}
			//FIXME: Really lame, need to figure out how to change font size in a sane way..
			tmp = tmp->getResized(UPoint(320, 50).getScaled());
			m_textFrame->changeBackground(tmp);

		m_textStrings.pop_back();
		m_textFadeIn = true;
	}
	if(m_textFadeIn)
		m_textFadeIn = m_textFrame->fadeIn(10);

	if(!m_soundStrings.empty() && (uint32_t)m_soundStrings.back().first == m_animLabel->getCurFrame())
	{
		Mix_Chunk *sound = NULL;
		while(!m_soundStrings.empty() && (uint32_t)m_soundStrings.back().first == m_animLabel->getCurFrame())
		{
			if(sound == NULL)
				sound = DataCache::Instance()->getGameData(m_soundStrings.back().second)->getSound().get();
			else
			{
				Mix_Chunk *sound1 = sound;
				Mix_Chunk *sound2 = DataCache::Instance()->getGameData(m_soundStrings.back().second)->getSound().get();
				Mix_Chunk *newChunk;
				if((newChunk = (Mix_Chunk*) malloc(sizeof(Mix_Chunk))) == NULL) {
					return -1;
				}
				
				newChunk->allocated = 1;
				newChunk->volume = sound1->volume;
				newChunk->alen = sound1->alen + sound2->alen;
				
				if((newChunk->abuf = (Uint8 *)malloc(newChunk->alen)) == NULL) {
					free(newChunk);
					return -1;
				}
				
				memcpy(newChunk->abuf, sound1->abuf, sound1->alen);
				memcpy(newChunk->abuf + sound1->alen, sound2->abuf, sound2->alen);
				sound = newChunk;
				Mix_FreeChunk(sound1);
				Mix_FreeChunk(sound2);
			}
			m_soundStrings.pop_back();
		}
		SoundPlayer::Instance()->playSound(sound);
	}

	return 0;
}
