#include "Entity2.h"

Entity2::Entity2() {}

Entity2::~Entity2() {}

Sprite* Entity2::getSprite() { return this->sprite; }

void Entity2::bindSprite(Sprite* sprite, INT32 layer)
{
	this->sprite = sprite;
	this->sprite->setGlobalZOrder(layer);

	Size size = this->sprite->getContentSize();
	this->setAnchorPoint(Point(0.5f, 0.5f));
	this->setContentSize(size);
	this->setPosition(Point(0, 0));

	this->addChild(this->sprite);
	this->sprite->setPosition(Point(size.width / 2, size.height / 2));
}



