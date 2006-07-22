#ifndef DUNE_GUI_VBOX_H
#define DUNE_GUI_VBOX_H

#include "Container.h"

class VBox : public Container
{
    public:
        virtual void reshape();
        virtual void fit(int margin = 0);
};

#endif // DUNE_GUI_VBOX_H




