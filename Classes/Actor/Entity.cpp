#include "Entity.h"

Entity::Entity() : attack(3), armor(5), HP(5), MP(5) { this->sprite = nullptr; }

Entity::Entity(Sprite* sprite) : attack(3), armor(5), HP(5), MP(5) {
  this->sprite = sprite;
  this->addChild(this->sprite);
}

Entity::~Entity() {}

Sprite* Entity::GetSprite() { return this->sprite; }

void Entity::BindSprite(Sprite* sprite) {
  this->sprite = sprite;
  this->addChild(this->sprite);
}


