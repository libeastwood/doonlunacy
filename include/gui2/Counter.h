#ifndef DUNE_GUI2_COUNTER_H
#define DUNE_GUI2_COUNTER_H

#include "gui2/Frame.h"

class Counter : public Frame
{
  public:
    Counter(int min, int max, int value);

    void increase();
    void decrease();
    int getValue() { return m_value; }

  private:
    int m_min,
        m_max,
	m_maxlen,
        m_value,
	m_bgColor;

    void drawDigits();

};
#endif // DUNE_HSCALE_H

