#ifndef _ENEMYCONTROLLER_H_
#define _ENEMYCONTROLLER_H_

#include "cocos2d.h"
#include "Enemy.h"

USING_NS_CC;

constexpr INT32 MAX_ENEMY_NUM = 10;
constexpr INT32 MIN_ENEMY_NUM = 4;

class EnemyController : public Node {
 public:
  CREATE_FUNC(EnemyController);

  virtual bool init();
  virtual void update(float delta);

  void bindKnight(Knight* knight);

private:
  void createEnemy();

private:
  Knight* knight = nullptr;

 private:
  Vector<Enemy*> vecEnemy;
};

#endif
