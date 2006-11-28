#ifndef DUNE_GUI2_LABEL_H
#define DUNE_GUI2_LABEL_H

#include "gui2/Widget.h"
#include <string>

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
        @param bgcolour sets background colour of label. 115(dune yellow) by default
    */
    Label(std::string caption, int bgcolour = 115);

    ~Label();
    //@}

    virtual void draw(SDL_Surface* dest, SPoint off);

protected:

    SDL_Surface * m_surface;    
    std::string m_caption;
};

#endif //DUNE_GUI2_LABEL_H
