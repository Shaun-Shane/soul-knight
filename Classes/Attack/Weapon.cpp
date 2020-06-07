#include "Weapon.h"


Weapon::Weapon(){}

bool Weapon::init()
{ 
  return true;	
}

Weapon::~Weapon(){}

void Weapon::bindWeapon(Weapon* weapon) 
{
  this->bindSprite(weapon->sprite, LayerPlayer + 1);
  this->attack = weapon->attack;
  this->fireSpeed = weapon->fireSpeed;
}

void Weapon::setFireSpeed(float fireSpeed)
{
  this->fireSpeed = fireSpeed;
}

float Weapon::getFireSpeed() { return this->fireSpeed; }
void Weapon::setAttack(INT32 attack) { this->attack = attack; }
INT32  Weapon::getAttack() { return this->attack; }

Bullet* Weapon::createBullet(Vec2 speed,INT32 firePower)
{
  Bullet* bullet = Bullet::create();
  bullet->setBulletSpeed(speed);
  bullet->bindSprite(Sprite::create("Bullet//pistol.png"), 12);

  if (speed.x == 0 && speed.y > 0)  bullet->getSprite()->setRotation(-90.0f);
  else if (speed.x < 0)   bullet->getSprite()->setRotation(-(180.0f + 180.0f * atan(speed.y / speed.x) / PI));
  else if (speed.x == 0 && speed.y < 0)   bullet->getSprite()->setRotation(-270.0f);
  else    bullet->getSprite()->setRotation(-(180.0f * atan(speed.y / speed.x) / PI));
  bullet->setAttack(firePower);

  return bullet;
}