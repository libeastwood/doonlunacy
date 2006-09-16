//#include "../sand.h"
//#include "../Items.h"

#include "units/SardaukarClass.h"
#include "PlayerClass.h"
#include "data.h"

SardaukarClass::SardaukarClass(PlayerClass* newOwner) : TrooperClass(newOwner)
{
//	TrooperClass::TrooperClass(newOwner);
	baseID = itemID = Unit_Sardaukar;
	
	armour = 1;
	maxHealth = 100;
	bulletType[0] = Bullet_SmallRocketHeavy;
	speed = 0.18;
}
SardaukarClass::~SardaukarClass()
{

}
