#ifndef DUNE_GUI2_LABEL_H
#define DUNE_GUI2_LABEL_H

#include "gui2/Widget.h"
#include "pakfile/Animation.h"
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
		@param textColour sets text colour of label. 49(dune red) by default
        @param bgColour sets background colour of label. 115(dune yellow) by default
    */
    Label(std::string caption, int textColour = 49, int bgColour = 115);

    ~Label();
    //@}

    virtual void draw(Image * dest, SPoint off);

protected:

    ImagePtr m_surface;    
    std::string m_caption;
};

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
