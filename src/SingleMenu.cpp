#include "SingleMenu.h"

#include "Application.h"
#include "houses.h"
//#include "DataFile.h"
#include "Settings.h"
#include "HouseChoiceMenu.h"

#include "boost/bind.hpp"


SingleMenuState::SingleMenuState()
{
    m_butCampaign = new BoringButton("Campaign");
    m_butCampaign->setSize(SPoint(bw, bh));
    m_butCampaign->onClick.connect(
            boost::bind(&SingleMenuState::doCampaign, this) );
   
    m_vbox->addChild(m_butCampaign);

    m_butCustom = new BoringButton("Custom Game");
    m_butCustom->setSize(SPoint(bw,bh));
    m_butCustom->onClick.connect(
            boost::bind(&SingleMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butCustom);

    m_butSkirmish = new BoringButton("Skirmish");
    m_butSkirmish->setSize(SPoint(bw, bh));
    m_butSkirmish->onClick.connect(
            boost::bind(&SingleMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butSkirmish);

    m_butLoad = new BoringButton("Load Game");
    m_butLoad->setSize(SPoint(bw, bh));
    m_butLoad->onClick.connect(
            boost::bind(&SingleMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butLoad);

    m_butCancel = new BoringButton("Cancel");
    m_butCancel->setSize(SPoint(bw, bh));
    m_butCancel->onClick.connect(
            boost::bind(&SingleMenuState::doCancel, this) );
   
    m_vbox->addChild(m_butCancel);

}

SingleMenuState::~SingleMenuState()
{
    delete m_butCampaign;
    delete m_butCustom;
    delete m_butSkirmish;
    delete m_butLoad;
    delete m_butCancel;
}

void SingleMenuState::doCampaign()
{
	printf("Campaign\n");
    mp_parent->PushState( new HouseChoiceMenuState() );

}
void SingleMenuState::doSkirmish()
{
    printf("Skirmish\n");
}

void SingleMenuState::doCancel()
{
    assert(mp_parent != NULL);
    PopState();
}

