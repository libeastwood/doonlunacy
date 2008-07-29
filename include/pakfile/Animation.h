#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

#include <SDL.h>
#include "Gfx.h"

#include <vector>

class Animation
{
public:
	Animation();
	~Animation();
	
	ImagePtr getFrame(double resizeRatio = 2.0);
	void setFrameRate(double FrameRate) {
		if(FrameRate == 0.0) {
			FrameDurationTime = 1;
		} else {
			FrameDurationTime = (int) (1000.0/FrameRate);
		}
	}
	
	void addFrame(Image * newFrame, bool SetColorKey = false);

private:
    typedef struct {
        bool resized;
        ImagePtr img;
    } animFrame;

	Uint32 CurFrameStartTime;
	Uint32 FrameDurationTime;
	Uint32 curFrame;
	std::vector<animFrame*> m_frames;
};

#endif // ANIMATION_H_INCLUDED

