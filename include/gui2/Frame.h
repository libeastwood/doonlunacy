#ifndef DUNE_GUI2_FRAME_H
#define DUNE_GUI2_FRAME_H

#include <string>
#include <vector>

#include "gui2/Container.h"

/* Frame widget
*/
class Frame : public Container
{
public:
    //! @name Constructors & Destructor
    //@{

    /*! 
        @param image image..
    */
    Frame(Image *image, UPoint size, GuiPic_enum nw, GuiPic_enum ne,
            GuiPic_enum sw, GuiPic_enum se, Uint16 edgeDistance = 0);
    Frame(Uint32 color, ConstUPoint size, GuiPic_enum nw, GuiPic_enum ne,
            GuiPic_enum sw, GuiPic_enum se, Image *background = NULL, Uint16 edgeDistance = 0);
    Frame(Image *image);
    Frame();

    void drawFrame(ConstUPoint size, GuiPic_enum nw, GuiPic_enum ne,
            GuiPic_enum sw, GuiPic_enum se, Uint16 edgeDistance);

    ~Frame();

    virtual void draw(Image *dest, SPoint off);

    UPoint getPictureSize() const
    {
        return m_surface->getSize();
    }

    ImagePtr getPicture();
    void changeBackground(Image *background);
    //@}

protected:
    ImagePtr m_surface;
    Container *m_container;
};

class Frame1 : public Frame
{
public:
    Frame1(Image *image, UPoint size = NULL, Uint16 edgeDistance = 0);
    Frame1(Uint32 color, ConstUPoint size, Image *background = NULL, Uint16 edgeDistance = 0);
};

class Frame2 : public Frame
{
public:
    Frame2(Image *image, UPoint size = NULL, Uint16 edgeDistance = 0);
    Frame2(Uint32 color, ConstUPoint size, Image *background = NULL, Uint16 edgeDistance = 0);

};

class Frame3 : public Frame
{
public:
    Frame3(Image *image, UPoint size = NULL, Uint16 edgeDistance = 0);
    Frame3(Uint32 color, ConstUPoint size, Image *background = NULL, Uint16 edgeDistance = 0);

};

#endif //DUNE_GUI2_FRAME_H

