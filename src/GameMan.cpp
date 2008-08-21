#include "GameMan.h"

GameMan::GameMan()
{
    Init();
}

GameMan::~GameMan()
{
    Clear();
}

void GameMan::Init()
{
    m_players = new Players;
    m_structures = new Structures;
    m_units = new Units;
	m_objectTree = new ObjectTree();
    m_bulletList = new Bullets;
/*
    m_bulletList->push_back(new BulletClass(NULL, UPoint(50,50), UPoint(498, 352), (int)Bullet_LargeRocket, false)); 
    m_bulletList->push_back(new BulletClass(NULL, UPoint(50,200), UPoint(498, 352), (int)Bullet_LargeRocket, false)); 
    m_bulletList->push_back(new BulletClass(NULL, UPoint(200,50), UPoint(498, 352), (int)Bullet_LargeRocket, false)); 
    m_bulletList->push_back(new BulletClass(NULL, UPoint(400,400), UPoint(498, 352), (int)Bullet_LargeRocket, false)); 
    m_bulletList->push_back(new BulletClass(NULL, UPoint(200,400), UPoint(498, 352), (int)Bullet_LargeRocket, false)); 
*/
}

void GameMan::Clear()
{
    for (unsigned int i = 0; i < m_players->size(); i++)
    {
        delete m_players->at(i);
    }
    m_players->clear();
    
    for (unsigned int i = 0; i < m_units->size(); i++)
    {
        UnitClass* unit = m_units->back(); 
        m_objectTree->RemoveObject(unit->getObjectID());
    }
    m_units->clear();
    
    delete m_map;
}
void GameMan::Update(float dt)
{
    Units::iterator unit = m_units->begin();
    UnitClass* tmp;
    while (unit != m_units->end())
    {
        tmp = *unit;
        if (tmp->clearObject())
        {
            m_objectTree->RemoveObject(tmp->getObjectID());
            m_units->remove(tmp);
            unit++;
        }
        else
        {
            unit++;
        }
    }
    
    for (unit = m_units->begin(); unit != m_units->end(); unit++)
    {
        (*unit)->update(dt);
    }
    
    Bullets::iterator bullet;
    for (bullet = m_bulletList->begin(); bullet != m_bulletList->end(); bullet++)
    {
        (*bullet)->updatePosition();
    }
}
