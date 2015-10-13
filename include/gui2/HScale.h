#ifndef DUNE_HSCALE_H
#define DUNE_HSCALE_H

#include "gui2/Button.h"
#include "gui2/Container.h"
#include "gui2/Label.h"

class HScale : public Container
{
  public:
    HScale(int min, int max, int value);
    ~HScale();

    void draw(Image& dest, SPoint off);

  private:
    int m_min,
        m_max,
        m_value;

    //! Shows current value for widget
    Label* m_label;
    BoringButton *m_butPlus, *m_butMinus;
    
    void increase();
    void decrease();

};
#endif // DUNE_HSCALE_H
