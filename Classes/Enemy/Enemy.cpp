#include "Enemy.h"
#include "Scene/BattleRoom.h"

Enemy::Enemy() { enemyIsAlive = false; }

Enemy::~Enemy() {}

bool Enemy::init() { 
  this->setHP(5);
  return true; 
}

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

void Enemy::patrolRoute(const BattleRoom* battleRoom, Knight* knight) {
  const Point enemyPos = this->getPosition();

  if (paceCount % 40) {
    this->setPosition(
        Point(enemyPos.x + 3 * DIRX[wayOfPace], enemyPos.y + 3 * DIRY[wayOfPace]));
    //3.0f是合移动速度
    paceCount++;
    return;
  }

  paceCount = 1;  //设置为1，避免下一轮被paceCount%40识别为false
  wayCanBeSelected.clear();

  for (INT32 dir = 0; dir < CNTDIR; dir++) { // 4个可以走的方向
    Point upLeftPos = battleRoom->getUpleftVertex();
    Point downRightPos = battleRoom->getDownRightVertex();

    if (upLeftPos.x  < enemyPos.x + DIRX[dir] * 120 && 
        enemyPos.x + DIRX[dir] * 120 < downRightPos.x &&
        downRightPos.y < enemyPos.y + DIRY[dir] * 120 &&
        enemyPos.y + DIRY[dir] * 120 < upLeftPos.y) {  //判断边界 40步=120.0f
      wayCanBeSelected.push_back(dir);
    }
  }  //选取可以走的方向

  srand(static_cast<unsigned int>(time(nullptr)));

  assert(wayCanBeSelected.size() != 0);

  wayOfPace = wayCanBeSelected[rand() % wayCanBeSelected.size()];
  this->setPosition(
      Point(enemyPos.x + 3 * DIRX[wayOfPace], enemyPos.y + 3 * DIRY[wayOfPace]));

}  //在没探测到骑士的时候正常的巡逻路线

void Enemy::aiOfEnemy(Knight* knight, const BattleRoom* battleRoom) {
  if (knight == nullptr || battleRoom == nullptr) {
    return;
  }

  const Point enemyPos = this->getPosition();
  const Point knightPos = knight->getPosition();
  const float disBetweenEnemyAndKnight =
      enemyPos.getDistance(knightPos);  //获取二者距离，用于后续判断

  if (disBetweenEnemyAndKnight > SIGHTRANGE) {
    patrolRoute(battleRoom, knight);
  } else {
    paceCount = 0;
    wayOfPace = -1;
    if (disBetweenEnemyAndKnight > ATTACKRANGE) {
      this->setPosition(Point(enemyPos.x + 3.0f * (knightPos.x - enemyPos.x) /
                                               disBetweenEnemyAndKnight,
                              enemyPos.y + 3.0f * (knightPos.y - enemyPos.y) /
                                               disBetweenEnemyAndKnight));
    } else {
      attackTheKnight(knight, disBetweenEnemyAndKnight);
    }
  }
}

void Enemy::attackTheKnight(Knight* knight,
                                float disBetweenEnemyAndKnight) {
  if (disBetweenEnemyAndKnight < ATTACKRANGE) {
    if (attackTimeCount % 40 == 0) {
      knight->deductHP(3);
      log("%d", knight->getHP());
      attackTimeCount = 1;
    }
    attackTimeCount++;
    return;
  }

  else {
    const Point enemyPos = this->getPosition();
    const Point knightPos = knight->getPosition();
    //this->setPosition(Point(  
    //    enemyPos.x + 3.0f * (knightPos.x - enemyPos.x) / disBetweenEnemyAndKnight,
    //    enemyPos.y + 3.0f * (knightPos.y - enemyPos.y) / disBetweenEnemyAndKnight));
  }  //等武器那一块出来加上武器
}