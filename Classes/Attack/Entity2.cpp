#include "Entity2.h"

<<<<<<< Updated upstream
Entity2::Entity2()
{
	sprite = NULL;
}

Entity2::~Entity2()
{
}

Sprite* Entity2::getSprite()
{
	return this->sprite;
}

void Entity2::bindSprite(Sprite* sprite)
{
	this->sprite = sprite;
}

void Entity2::setAttack(INT32 attack)
{
	this->attack = attack;
}



INT32 Entity2::getAttack()
{
	
	return this->attack;
}
=======
Entity2::Entity2() {}

Entity2::~Entity2(){}

Sprite* Entity2::getSprite() {  return this->sprite;  }

void Entity2::bindSprite(Sprite* sprite, INT32 layer)
{
  this->sprite = sprite;
  this->sprite->setGlobalZOrder(layer);
  this->addChild(this->sprite);
}

void Entity2::setAttack(INT32 attack) { this->attack = attack; }

INT32 Entity2::getAttack() { return this->attack; }

>>>>>>> Stashed changes

