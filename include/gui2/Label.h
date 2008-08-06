#ifndef DUNE_GUI2_LABEL_H
#define DUNE_GUI2_LABEL_H

#include <string>
#include <vector>

#include "gui2/Widget.h"
#include "Container.h"

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
    Label(std::string caption="", int textColor = 49, int bgColor = 115, int maxLineLength = -1);
    void setLabel(std::string);
    void redraw();

    ~Label();
    //@}

    virtual void draw(Image * dest, SPoint off);

protected:
    virtual void drawBackground(Uint16 textw, Uint16 texth, Uint16 numLines = 1);
    ImagePtr m_surface;
    std::string m_caption;

private:
    std::vector<std::string> splitString(std::string ParseString, int maxLineLength);
    int m_textColor,
    m_bgColor,
    m_maxLineLength;
    bool m_redraw;
};


class TransparentLabel : public Label
{
public:
    TransparentLabel(std::string caption="", int textColor = 49, int maxLineLength = -1);
    ~TransparentLabel();

protected:
    virtual void drawBackground(Uint16 textw, Uint16 texth, Uint16 numLines = 1);

};

class GraphicsLabel : public Label
{
public:
    GraphicsLabel(Image *background, std::string caption="", int textColor = 49, int maxLineLength = -1);
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
		AnimationLabel(Animation* pAnim);
		
		~AnimationLabel();


		virtual void draw(Image * dest, SPoint off);

	private:
		Animation* m_anim;
	
};
#endif //DUNE_GUI2_LABEL_H
