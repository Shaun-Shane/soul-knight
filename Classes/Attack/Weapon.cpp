#include "Weapon.h"

Weapon::Weapon(){}

bool Weapon::init()
<<<<<<< Updated upstream
{
  
    return true;
}




void Weapon::bindWeapon(Weapon* weapon)
{
    this->sprite = weapon->getSprite();
    this->attack = weapon->getAttack();
    this->speed = weapon->getSpeed();
}



Bullet*& Weapon::createBullet(Vec2 speed, Vec2 curPos, BattleRoom* atBattleRoom, Hall* atHall)  //to do再添加两个参数 atBattleRoom和atHall
{
    Bullet* bullet = Bullet::create();
    bullet->bindSprite(Sprite::create("bullet.png"));
    bullet->setPosition(curPos);
    bullet->setSpeed(speed);
    bullet->getSprite()->setZOrder(TOP);
    if (atBattleRoom == nullptr) {
        assert(atHall != nullptr);
        atHall->addChild(bullet);
        atHall->getVecPlayerBullet().pushBack(bullet);
    }
    else {
        assert(atBattleRoom != nullptr);
        atBattleRoom->addChild(bullet);
        atBattleRoom->getVecPlayerBullet().pushBack(bullet);
    }
    
    
    return bullet;
}

float Weapon::getSpeed()
{
	return this->speed;
}

void Weapon::setSpeed(float m)
{
    this->speed = m;
}

Weapon::~Weapon() {}
=======
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

>>>>>>> Stashed changes
