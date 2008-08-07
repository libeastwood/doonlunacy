#ifndef DUNE_ANIMATION_H
#define DUNE_ANIMATION_H

#include "Gfx.h"

class Animation
{
public:
	Animation();
	~Animation();
	
	ImagePtr getFrame();
	void setFrameRate(float frameRate) {
		if(frameRate == 0.0) {
			frameDurationTime = 1;
		} else {
			frameDurationTime = (int) (1000.0/frameRate);
		}
	}
	
	void addFrame(SDL_Surface *newFrame, bool SetColorKey = false);

private:
	int curFrameStartTime;
	int frameDurationTime;
	int curFrame;
	int numFrames;
	std::vector<ImagePtr> frameVector;
};

#endif // EASTWOOD_ANIMATION_H
