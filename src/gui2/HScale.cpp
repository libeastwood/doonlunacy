#include "boost/bind.hpp"

#include "gui2/HScale.h"
#include "Strings.h"
HScale::HScale(int min, int max, int value)
{
    m_min = min;
    m_max = max;
    m_value = value;

    setSize(SPoint(180, 20));
        
    m_butPlus = new BoringButton("+");
    m_butPlus->setSize(SPoint(20, 20));
    m_butPlus->onClick.connect(
            boost::bind(&HScale::increase, this) );
            
    m_butMinus = new BoringButton("-");
    m_butMinus->setSize(SPoint(20,20));
    m_butMinus->setPosition(SPoint(20,0));
    m_butMinus->onClick.connect(
            boost::bind(&HScale::decrease, this) );

    m_label = new Label(toString(value));
    m_label->setSize(SPoint(30,20));
    m_label->setPosition(SPoint(w - m_label->w, 0));
    addChild(m_butPlus);
    addChild(m_butMinus);
    addChild(m_label);

}

HScale::~HScale()
{

}

void HScale::draw(Image * dest, SPoint off)
{
    Rect scale(off.x + x + 45, off.y + y+ 7, 0, 5);
    scale.w = (int)((m_value * 100)/ m_max);
    
    Container::draw(dest, off);
    dest->fillRect(52, scale);
    scale.y--;
    scale.w = 100;
    scale.h+=2;
    dest->drawRect(scale, 0);
}


void HScale::decrease()
{
    m_value--;
    if (m_value < m_min)
        m_value = m_min;
    m_label->setLabel(toString(m_value));
}

void HScale::increase()
{
    m_value++;
    if (m_value > m_max)
        m_value = m_max;
    m_label->setLabel(toString(m_value));
}
