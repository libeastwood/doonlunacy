#ifndef DUNE_DRAWIMAGE_H
#define DUNE_DRAWIMAGE_H

#include "Gfx.h"
#include "Definitions.h"

class DrawImage : public Image
{
    public:
        DrawImage(ConstUPoint size, Uint32 color = -1);
        DrawImage(Image *image);
	~DrawImage();

        void drawBorders(GuiPic_enum nw, GuiPic_enum ne, GuiPic_enum sw,
			GuiPic_enum se, Uint16 edgeDistance = 0);
        void drawBorders1(Uint16 edgeDistance = 0)
	{
		drawBorders(UI_Corner1NW, UI_Corner1NE, UI_Corner1SW,
				UI_Corner1SE, edgeDistance);
	}

        void drawBorders2(Uint16 edgeDistance = 0)
	{
		drawBorders(UI_Corner2NW, UI_Corner2NE, UI_Corner2SW,
				UI_Corner2SE, edgeDistance);
	}

        void drawBorders3(Uint16 edgeDistance = 0)
	{
		drawBorders(UI_Corner3NW, UI_Corner3NE, UI_Corner3SW,
				UI_Corner3SE, edgeDistance);
	}

	void drawTiles(Image *tile);

};
#endif // DUNE_DRAWIMAGE_H
