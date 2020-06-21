#include "prop.h"

Prop::Prop() {}

Prop::~Prop() {}

bool Prop::init() { return true; }

INT32 Prop::getPropIndex() { return this->propIndex; }

void Prop::setPropIndex(INT32 index) { this->propIndex = index; }

void Prop::useProps(Knight* knight)
{
	switch (this->propIndex)
	{
	case 1:
		INT32 curHP;
		curHP = knight->getHP() + 2;
		knight->setHP(curHP);
		break;
	case 2:
		INT32 curMP;
		curMP = knight->getMP() + 20;
		knight->setMP(curMP);
		break;
	case 3:
		knight->addGold(5);
		break;
	case 4:
		knight->setArmor(knight->getMaxArmor());
	}
}
