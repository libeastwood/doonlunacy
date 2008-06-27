#include "SkirmishMenu.h"

#include "Application.h"
#include "houses.h"
//#include "DataFile.h"
#include "Settings.h"

#include "boost/bind.hpp"


SkirmishMenuState::SkirmishMenuState()
{
    m_butCancel = new BoringButton("Cancel");
    m_butCancel->setSize(SPoint(bw, bh));
    m_butCancel->onClick.connect(
            boost::bind(&SkirmishMenuState::doCancel, this) );
   
    m_vbox->addChild(m_butCancel);

}

SkirmishMenuState::~SkirmishMenuState()
{
}

void SkirmishMenuState::doCancel()
{
    assert(mp_parent != NULL);
    PopState();
}

