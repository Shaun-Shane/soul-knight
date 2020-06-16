#ifndef _BOSS_H_
#define _BOSS_H_

#include "Enemy.h"
#include "Actor/Knight.h"
#include "Const/Const.h"
#include "cocos2d.h"
#include <vector>

class Boss :public Enemy {
	static constexpr INT32 HEAVYATTACKRANGE = 400;//最大强力攻击距离
	static constexpr INT32 MAXFLASHRANGE = 300;//最大瞬移距离
	static constexpr INT32 UNISKITIMEINTERVAL = 180;//两次发大招的时间间隔
	static constexpr INT32 CLOSECOMBATRANGE = 40;//近距离攻击距离
	static constexpr INT32 RIGHT = 0;
	static constexpr INT32 UP = 1;
	static constexpr INT32 LEFT = 2;
	static constexpr INT32 DOWN = 3;
public:
	Boss();
	~Boss();

	CREATE_FUNC(Boss);
	virtual bool init();

	void aiOfBoss(Knight* knight, BattleRoom* battleRoom);//boundaryOfRoom是一个封装了边界的vector，按顺序依次应为右边界、上、左、下

private:
	void uniqueSkill(Knight* knight);

	void addHP();//加血绝招
	void heavilyAttackTheKnight(Knight* knight);//大范围攻击骑士的绝招
	void flashMove(Knight* knight);//瞬移至骑士坐标或尽力向骑士方向瞬移的绝招
private:
	INT32 uniSkiTimeCount;
	INT32 aiChoice;
	INT32 wayOfPace;//选择走的方向
	std::vector<INT32> wayCanBeSelected;//可供选择的行走方向
};

#endif
