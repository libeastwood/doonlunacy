#include "Definitions.h"

#include "gui2/Container.h"
#include "gui2/Frame.h"
#include "DataCache.h"

Frame::Frame(Image *image)
{
    m_surface.reset(image);
}

Frame::Frame()
{
    // Needs to be initialized with a proper surface..
    m_surface.reset(new Image(UPoint(1,1)));
}

void Frame::draw(Image *dest, SPoint off){
    if (!m_visible) return;

    m_surface->blitTo(dest, UPoint(off.x + x, off.y + y));

    WidgetList::iterator it;
    for ( it  = m_children.begin();
          it != m_children.end();
          ++it )
    {
        (*it)->draw(dest, SPoint(off.x + x, off.y + y));
    };
}

Frame::~Frame()
{

}

void Frame::changeBackground(Image *background)
{
    m_surface.reset(background);
}
