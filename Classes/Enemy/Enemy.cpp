#include "Enemy.h"

Enemy::Enemy() { enemyIsAlive = false; }

Enemy::~Enemy() {}

bool Enemy::init() { return true; }

void Enemy::show() {
  if (getSprite() != nullptr) {
    setVisible(true); //可见
    enemyIsAlive = true;
  } 
}

void Enemy::hide() {
  if (getSprite() != nullptr) {
    setVisible(false); //不可见
    reset(); //重置敌人数据
    enemyIsAlive = false;
  }
}

void Enemy::reset() {
  if (getSprite() != nullptr) {
    // randomly reset positon
    // need update
    setPosition(Point(800 + CCRANDOM_0_1() * 2000, 200 - CCRANDOM_0_1() * 100));
  }
}

bool Enemy::isAlive() { return enemyIsAlive;}

//碰撞检查 可能没用
bool Enemy::isCollideWithKnight(Knight * knight) {
  //获取碰撞对象的bounding box
  Rect entityRec = knight->getBoundingBox();

  Point enemyPos = getPosition();

  //判断bounding box 和 怪物中心点是否有交集
  return entityRec.containsPoint(enemyPos);
}