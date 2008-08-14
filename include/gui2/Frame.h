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

	void changeBackground(ImagePtr background);

    //@}

};

#endif //DUNE_GUI2_FRAME_H
