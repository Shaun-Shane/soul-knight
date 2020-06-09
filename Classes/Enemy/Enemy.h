#ifndef _ENEMY_H_
#define _ENEMY_H_
#include <vector>

#include "Actor/Entity.h"
#include "Actor/Knight.h"
#include "Const/Const.h"

class Enemy : public Entity {
  static constexpr INT32 SIGHTRANGE = 260;
  static constexpr INT32 ATTACKRANGE = 150;

 public:
  Enemy();
  ~Enemy();
  CREATE_FUNC(Enemy);
  virtual bool init();

 public:
  void show();
  void hide();
  void reset();
  bool isAlive();
  bool isCollideWithKnight(Knight* knight);

  void aiOfEnemy(Knight* knight, const BattleRoom* battleRoom);  

 private:
  void patrolRoute(const BattleRoom* battleRoom, Knight* knight);
  void attackTheKnight(Knight* knight, float disBetweenEnemyAndKnight);

 private:
  bool enemyIsAlive;

  INT32 paceCount = 0;  //用于保证至少20步都在走同一方向
  INT32 wayOfPace = -1;  //选择走的方向
  INT32 attackTimeCount = 1;
  std::vector<INT32> wayCanBeSelected;  //可供选择的行走方向
};

#endif