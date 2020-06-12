#ifndef _ENEMY_H_
#define _ENEMY_H_
#include <vector>

#include "Actor/Entity.h"
#include "Actor/Knight.h"
#include "Const/Const.h"

class Enemy : public Entity {
  static constexpr INT32 SIGHTRANGE = 260;
  INT32 ATTACKRANGE = 150;//将ATTACKRANGE修改为可自行设置，方便创造不同怪物，默认为150

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

  void setType(int type);

  void aiOfEnemy(Knight* knight, const BattleRoom* battleRoom);  

  INT32 startCount;

 private:
  void patrolRoute(const BattleRoom* battleRoom, Knight* knight);
  void attackTheKnight(Knight* knight, float disBetweenEnemyAndKnight, const BattleRoom* battleRoom);
  void setAttackRange();
  bool inRoom(const BattleRoom* battleRoom,Point myPos);

private:
    void archerAttack(Knight* knight, float disBetweenEnemyAndKnight);//当敌人设定类型为弓箭手时调用的攻击函数
	void boarAttack(Knight* knight, float disBetweenEnemyAndKnight, const BattleRoom* battleRoom);//当敌人设定类型为野猪时调用的攻击函数
    INT32 boarRushCount;//记录野猪冲的时长，满了一定就停一会儿，以免它太具攻击性了
    bool boarRest = false;//与上面一行变量配套
    bool boarHaveAttacked = false;//与boarAttack配套的bool值
    float boarBumpDirection[2] = { 1.0,0.0 };

 private:
  bool enemyIsAlive;

  INT32 enemyType = 0;

  INT32 paceCount = 0;  //用于保证至少20步都在走同一方向
  INT32 wayOfPace = -1;  //选择走的方向
  INT32 attackTimeCount = 1;
  std::vector<INT32> wayCanBeSelected;  //可供选择的行走方向
  float shiftSeed=0.0f;
  INT32 followCount = 0;
};

#endif