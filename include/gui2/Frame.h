#ifndef DUNE_GUI2_FRAME_H
#define DUNE_GUI2_FRAME_H

#include "gui2/Container.h"

#include <string>
#include <vector>

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
	Frame(ImagePtr image);
    Frame();

    ~Frame();

    virtual void draw(Image *dest, SPoint off);

    UPoint getPictureSize() const
    {
        return m_surface->getSize();
    }

    void changeBackground(Image *background);
	void changeBackground(ImagePtr background);

    //@}

protected:
    ImagePtr m_surface;
};

#endif //DUNE_GUI2_FRAME_H
