include "Animation.h"

Animation::Animation() {
	furFrameStartTime = SDL_GetTicks();
	frameDurationTime = 1;
	numFrames = 0;
	curFrame = 0;
	frame = NULL;
}

Animation::~Animation() {
	if(frame != NULL) {
		for(int i=0; i < numFrames; i++) {
		//	SDL_FreeSurface(Frame[i]);
//			Frame[i] = NULL;
		}
//		free(frame);
	}
}

ImagePtr Animation::getFrame() {
	if(frame == NULL) {
		return NULL;
	}
	
	if((SDL_GetTicks() - curFrameStartTime) > frameDurationTime) {
		curFrameStartTime = SDL_GetTicks();
		curFrame++;
		if(curFrame >= numFrames) {
			curFrame = 0;
		}
	}
	return frame[curFrame];
}

void Animation::addFrame(SDL_Surface * newFrame, bool SetColorKey) {
	if((frame = (SDL_Surface **) realloc(frame,sizeof(SDL_Surface *) * (numFrames+1))) == NULL) {
		perror("Animation::addFrame()");
		exit(EXIT_FAILURE);
	}
	
		Frame[numFrames] = newFrame;
	
	if(SetColorKey == true) {
		SDL_SetColorKey(Frame[numFrames], SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);
	}
	numFrames++;
}
