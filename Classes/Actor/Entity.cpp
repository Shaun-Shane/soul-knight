#include "Entity.h"

Entity::Entity() : attack(0), HP(0), attackSpeed(.0) { this->sprite = nullptr; }

Entity::Entity(INT32 _attack, INT32 _HP, float _attackSpeed)
    : attack(_attack), HP(_HP), attackSpeed(_attackSpeed) {
  this->sprite = nullptr;
}

Entity::Entity(Sprite* sprite) : attack(3), HP(5), attackSpeed(0.5f) {
  this->sprite = sprite;
  this->addChild(this->sprite);
}

Entity::~Entity() {}

Sprite* Entity::getSprite() { return this->sprite; }

void Entity::bindSprite(Sprite* sprite) {
  this->sprite = sprite;
  this->addChild(this->sprite);

  //将entity中心和sprite中心重合
  /*Size size = this->sprite->getContentSize();
  this->sprite->setPosition(Point(size.width / 2, size.height / 2));
  this->setContentSize(size);*/
}

INT32 Entity::getHP() { //return HP of this entity
  if (getSprite() == nullptr) {
    log("null Sprite !");
    return 0;
  }
  return this->HP;
}

void Entity::deductHP(INT32 delta) { //minus HP of this entity
  if (getSprite() == nullptr) return;
  this->HP -= delta;
}


