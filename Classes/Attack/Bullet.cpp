#include "Bullet.h"

Bullet::Bullet() {}


Bullet::~Bullet(){ }

bool Bullet::init()
{
	return true;
}



void Bullet::setSpeed(Vec2 speed)
{

	this->speed = speed;
}

Vec2 Bullet::getSpeed()
{
	return speed;
}

bool Bullet::isInScreen()
{
	Vec2 size = Director::getInstance()->getVisibleSize();
	Vec2 pos = this->getPosition();
	return (pos.x > 0 && pos.x < size.x&& pos.y>0 && pos.y < size.y);
}
