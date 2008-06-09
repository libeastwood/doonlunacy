#ifndef DUNE_MAPWIDGET_H
#define DUNE_MAPWIDGET_H

#include "gui2/Widget.h"
#include "Gfx.h"
#include "MapClass.h"
#include "ObjectClass.h"

typedef std::vector <ObjectClass*> List;

class MapWidget : public Widget {
  public:
	MapWidget();
	~MapWidget();
    
    void draw(Image * dest, SPoint off);
    bool handleMotion(SPoint p);
    bool handleKeyDown(SDL_keysym* key);

	void setMap(MapClass* map) { m_map = map; }
	void setStructureList(SList* list) { m_structureList = list; }
  private:
    MapClass * m_map;
    SList* m_structureList;
    SPoint m_speed;
    SPoint m_view;
};

#endif // DUNE_MAPWIDGET_H

