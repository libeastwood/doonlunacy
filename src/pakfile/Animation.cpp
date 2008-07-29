#include "pakfile/Animation.h"
#include <stdio.h>
#include <stdlib.h>
#include "Log.h"
Animation::Animation() {
	CurFrameStartTime = SDL_GetTicks();
	FrameDurationTime = 1;
	curFrame = 0;
}

Animation::~Animation() {
/*    if (!Frame.empty())
    {
        std::vector<Image*>::iterator iter;
        for( iter = Frame.begin(); iter != Frame.end(); iter++ ) 
        {
            delete(*iter);
        }
    }
*/
//	if(Frame != NULL) {
//		for(int i=0; i < NumFrames; i++) {
//			SDL_FreeSurface(Frame[i]->getSurface());
//			Frame[i] = NULL;
//		}
//		free(Frame);
//	}
}

ImagePtr  Animation::getFrame(double resizeRatio) {
	if(m_frames.empty()) {
        LOG_ERROR("Animation", "No frames loaded. Returning stub frame");
        return ImagePtr(new Image(UPoint(1,1)));
	}
	
	if((SDL_GetTicks() - CurFrameStartTime) > FrameDurationTime) {
		CurFrameStartTime = SDL_GetTicks();
		curFrame++;
		if(curFrame >= m_frames.size()) {
			curFrame = 0;
		}
	}
	
	animFrame * tmp = m_frames.at(curFrame);
	
	if (!tmp->resized)
    {
        tmp->img = tmp->img->getResized(resizeRatio);
        tmp->resized = true;
    }
    return tmp->img;
}

void Animation::addFrame(Image * newFrame, bool SetColorKey) {
	if(SetColorKey == true) {
        newFrame->setColorKey();
	}

    animFrame * tmp = new animFrame;
    tmp->resized = false;
    tmp->img = ImagePtr(newFrame);

    m_frames.push_back(tmp);
}
