#include "Entity2.h"

Entity2::Entity2() {}

Entity2::~Entity2(){}

Sprite* Entity2::getSprite() {  return this->sprite;  }

void Entity2::bindSprite(Sprite* sprite, INT32 layer)
{
  this->sprite = sprite;
  this->sprite->setGlobalZOrder(layer);
  this->addChild(this->sprite);

  Size size = this->sprite->getContentSize();
  this->sprite->setPosition(Point(size.width / 2, size.height / 2));
  this->setContentSize(size);
}



