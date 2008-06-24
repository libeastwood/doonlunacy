#ifndef DUNE_GUI2_FRAME_H
#define DUNE_GUI2_FRAME_H

#include <string>
#include <vector>

#include "gui2/Widget.h"

/* Frame widget
*/
class Frame : public Widget
{
public:
    //! @name Constructors & Destructor
    //@{

    /*! 
        @param image image..
    */
    Frame(Image *image, UPoint size, GuiPic_enum nw, GuiPic_enum ne,
            GuiPic_enum sw, GuiPic_enum se);
    Frame(Uint32 color, ConstUPoint size, GuiPic_enum nw, GuiPic_enum ne,
            GuiPic_enum sw, GuiPic_enum se, Image *background = NULL);
    
    void drawFrame(ConstUPoint size, GuiPic_enum nw, GuiPic_enum ne,
            GuiPic_enum sw, GuiPic_enum se);

    ~Frame();
    ImagePtr getPicture();
    //@}

protected:
    ImagePtr m_surface;    
};

class Frame1 : public Frame
{
public:
    Frame1(Image *image, UPoint size = NULL);
    Frame1(Uint32 color, ConstUPoint size, Image *background = NULL);
};

class Frame2 : public Frame
{
public:
    Frame2(Image *image, UPoint size = NULL);
    Frame2(Uint32 color, ConstUPoint size, Image *background = NULL);

};

class Frame3 : public Frame
{
public:
    Frame3(Image *image, UPoint size = NULL);
    Frame3(Uint32 color, ConstUPoint size, Image *background = NULL);

};

#endif //DUNE_GUI2_FRAME_H

