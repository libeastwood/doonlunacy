#ifndef DUNE_MAPWIDGET_H
#define DUNE_MAPWIDGET_H

#include "gui2/Widget.h"
#include "Gfx.h"
#include "MapClass.h"
#include "structures/StructureClass.h"
#include "units/UnitClass.h"
/*!
 *  Widget used to display battle scene
 *  Since we're not using global variables anymore, it's got to have pointers
 *  to structure, unit lists and map.
 */

class MapWidget : public Widget {
  public:
    //! @name constructor & destructor
    //@{
	MapWidget();
	~MapWidget();
    //@}
    void draw(Image * dest, SPoint off);
    
    //! @name widget methods to handle user input
    /*! @note It's possible to have more than one MapWidget at the same time,
     *        but there's a problem with handling keys. As soon as on widget
     *        returns true for event handling, Application will not query another
     *        widget. Unless we think of some explicit method of specifying which 
     *        MapWidget is to handle key down.
     */
    //@{
    bool handleMotion(SPoint p);
    bool handleKeyDown(SDL_keysym* key);
    //@}
    void setGameState(GameState* gs);

  private:
    GameState* m_gs;    
    MapClass * m_map;
    Structures* m_structures;
    Units* m_units;
    /*!  
     *  Scroll speed multiplied by BLOCKSIZE. E.g. m_speed equal to (1,1)
     *  means that every time widget is updated, visible area is scrolled by
     *  16 pixels in north-east direction.
     */
    
    SPoint m_speed;
    
    /*!
     *  Specifies offset of displayed map. E.g. m_view equal to (10,10)
     *  Means that the top left cell will have such coordinates.
     *  Bottom right corner is specified by Widget's width and height.
     *  Current view would therefore look like this:
     *  (m_view.x*BLOCKSIZE, m_view.y*BLOCKSIZE, m_view.x*BLOCKSIZE+w, m_view.y*BLOCKSIZE+h)
     *  @note It's a bit clumsy, so I'll have to edit it as soon as I've understood
     *        how everything works. Some sizes are in pixels, whereas some in cells.
     *        It's a bit confusing.
     */
    SPoint m_view;
};

#endif // DUNE_MAPWIDGET_H

