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

