#include "Weapon.h"

Weapon::Weapon(){}

bool Weapon::init()
{ 
  return true;	
}

Weapon::~Weapon(){}

void Weapon::bindWeapon(Weapon* weapon) 
{
  this->sprite = weapon->sprite;
  this->attack = weapon->attack;
  this->fireSpeed = weapon->fireSpeed;
}



void Weapon::setFireSpeed(float fireSpeed)
{
  this->fireSpeed = fireSpeed;
}

float Weapon::getFireSpeed() { return this->fireSpeed; }

Bullet* Weapon::createBullet(Vec2 speed,INT32 firePower)
{
  Bullet* bullet = Bullet::create();
  bullet->setBulletSpeed(speed);
  bullet->bindSprite(Sprite::create("Bullet//pistol.png"), 12);
  
  bullet->setAttack(firePower);

  return bullet;
  
}