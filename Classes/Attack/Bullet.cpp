#include "Bullet.h"

Bullet::Bullet(){}

Bullet::~Bullet(){}

bool Bullet::init() {  return true; }

Vec2 Bullet::getBulletSpeed() { return this->bulletSpeed; }

void Bullet::setBulletSpeed(Vec2 speed) { this->bulletSpeed = speed; }


bool Bullet::isInScreen()        ///等了解如何获取地图位置再写
{
  return true;
}


