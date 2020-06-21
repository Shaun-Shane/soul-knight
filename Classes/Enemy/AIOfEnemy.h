#ifndef _AIOFENEMY_H_
#define _AIOFENEMY_H_
#include <vector>
#include "Enemy.h"
#include "Const/Const.h"
#include "Scene/BattleRoom.h"

class AIOfEnemy {
	static constexpr INT32 SIGHTRANGE = 300;
	static constexpr INT32 ATTACKRANGE = 100;

public:
	AIOfEnemy();

	~AIOfEnemy();

	void bindEnemy(Enemy* enemy);

	void aiOfEnemy(
		Knight* knight, BattleRoom* battleRoom,
		const std::vector<float>&
		boundaryOfRoom);  // boundaryOfRoom是一个封装了边界的vector，按顺序依次应为右边界、上、左、下

private:
	void patrolRoute(const BattleRoom* battleRoom, Knight* knight,
		const std::vector<float>& boundaryOfRoom);
	void attackTheKnight(Knight* knight, INT32 disBetweenEnemyAndKnight);

private:
	Enemy* myEnemy;
	INT32 paceCount;                      //用于保证至少20步都在走同一方向
	INT32 wayOfPace;                      //选择走的方向
	INT32 attackTimeCount;
	std::vector<INT32> wayCanBeSelected;  //可供选择的行走方向
};
#endif  // _AIOFENEMY_H_