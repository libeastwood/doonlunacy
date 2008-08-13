#ifndef DUNE_GUI2_LABEL_H
#define DUNE_GUI2_LABEL_H

#include "gui2/Widget.h"

#include <string>
#include <vector>

/* Label widget using dune's fonts
*/
class Label : public Widget
{
public:
    //! @name Constructors & Destructor
    //@{

    /*! 
        Caption of label should always be set when constructing label
        @param caption std::string a caption of label
		@param textColor sets text color of label. 49(dune red) by default
        @param bgColor sets background color of label. 115(dune yellow) by default
        @param maxLineLength maximum length of a line (in characters)
    */
    Label(std::string caption="", int textColor = 49, int bgColor = 115, int maxLineLength = 0);
    void setLabel(std::string);
    void redraw();

    ~Label();
    //@}

    virtual void draw(Image * dest, SPoint off);

	void setResizeRatio(int resizeRatio) { m_resizeRatio = resizeRatio; }
	void setResize(bool resize) { m_resize = resize; }

protected:
    virtual void drawBackground(Uint16 textw, Uint16 texth, Uint16 numLines = 1);
    ImagePtr m_surface;
    std::string m_caption;
	int m_resizeRatio;
	bool m_resize;

private:
    uint32_t m_textColor,
    m_bgColor,
    m_maxLineLength;
    bool m_redraw;
};


class TransparentLabel : public Label
{
public:
    TransparentLabel(std::string caption="", int textColor = 49, int maxLineLength = 0);

protected:
    virtual void drawBackground(Uint16 textw, Uint16 texth, Uint16 numLines = 1);

};

class GraphicsLabel : public Label
{
public:
    GraphicsLabel(ImagePtr background, std::string caption="", int textColor = 49, int maxLineLength = 0);
    ~GraphicsLabel();

protected:
    virtual void drawBackground(Uint16 textw, Uint16 texth, Uint16 numLines = 1);

private:
    ImagePtr m_background;
};

class Animation;
class AnimationLabel : public Widget
{
	public:
		AnimationLabel(ConstUPoint origSize);
		AnimationLabel();
		~AnimationLabel();

		virtual void draw(Image * dest, SPoint off);

		ImagePtr getFrame();
		void setFrameRate(float frameRate) {
			if(frameRate == 0.0) {
				m_frameDurationTime = 1;
			} else {
				m_frameDurationTime = (int) (1000.0/frameRate);
			}
		}
	
		void addFrame(ImagePtr animFrame, bool setColorKey = false);
		uint32_t getNumFrames() { return m_animFrames.size(); }
		uint32_t getCurFrame() { return m_curFrame; }

	private:
		uint32_t m_curFrame,
				 m_curFrameStartTime,
				 m_frameDurationTime;
		UPoint m_origSize;
		std::vector<ImagePtr> m_animFrames;
};
#endif //DUNE_GUI2_LABEL_H
