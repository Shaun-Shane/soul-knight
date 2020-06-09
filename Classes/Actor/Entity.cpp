﻿#include "Entity.h"

Entity::Entity() : attack(0), HP(0), attackSpeed(.0f), moveSpeedX(.0f), moveSpeedY(.0f) { this->sprite = nullptr; }

Entity::Entity(INT32 _attack, INT32 _HP, float _attackSpeed, float _moveSpeedX, float _moveSpeedY)
    : attack(_attack), HP(_HP), attackSpeed(_attackSpeed), moveSpeedX(_moveSpeedX), moveSpeedY(_moveSpeedY) {
  this->sprite = nullptr;
}

Entity::Entity(Sprite* sprite) : attack(3), HP(5), attackSpeed(0.5f) {
  this->sprite = sprite;
  this->addChild(this->sprite);
}

Entity::~Entity() {}

Sprite* Entity::getSprite() { return this->sprite; }

void Entity::bindSprite(Sprite* sprite, INT32 layer) {
  this->sprite = sprite;
  this->sprite->setGlobalZOrder(layer);

  //将entity中心和sprite中心重合
  Size size = this->sprite->getContentSize();
  this->setContentSize(size); //设置大小

  this->setAnchorPoint(Point(0.5f, 0.5f)); //设置锚点
  setPosition(Point(.0f, .0f));


  this->addChild(sprite);
  sprite->setPosition(Point(size.width / 2, size.height / 2));
}

INT32 Entity::getHP() { //return HP of this entity
  if (getSprite() == nullptr) {
    log("null Sprite !");
    return 0;
  }
  return this->HP;
}

float Entity::getMoveSpeedX() { return moveSpeedX; }

float Entity::getMoveSpeedY() { return moveSpeedY; }

void Entity::deductHP(INT32 delta) { //minus HP of this entity
  if (getSprite() == nullptr) return;
  this->HP =this->HP- delta;
}

void Entity::setHP(INT32 HP) { this->HP = HP; }
