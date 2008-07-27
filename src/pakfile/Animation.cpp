#include "pakfile/Animation.h"
#include <stdio.h>
#include <stdlib.h>
Animation::Animation() {
	CurFrameStartTime = SDL_GetTicks();
	FrameDurationTime = 1;
	curFrame = 0;
}

Animation::~Animation() {
    if (!Frame.empty())
    {
        std::vector<Image*>::iterator iter;
        for( iter = Frame.begin(); iter != Frame.end(); iter++ ) 
        {
            delete(*iter);
        }
    }
//	if(Frame != NULL) {
//		for(int i=0; i < NumFrames; i++) {
//			SDL_FreeSurface(Frame[i]->getSurface());
//			Frame[i] = NULL;
//		}
//		free(Frame);
//	}
}

Image *  Animation::getFrame() {
	if(Frame.empty()) {
		return NULL;
	}
	
	if((SDL_GetTicks() - CurFrameStartTime) > FrameDurationTime) {
		CurFrameStartTime = SDL_GetTicks();
		curFrame++;
		if(curFrame >= Frame.size()) {
			curFrame = 0;
		}
	}
	return Frame.at(curFrame);
}

void Animation::addFrame(Image * newFrame, bool SetColorKey) {
	if(SetColorKey == true) {
        newFrame->setColorKey();
	}

    Frame.push_back(newFrame);
}
