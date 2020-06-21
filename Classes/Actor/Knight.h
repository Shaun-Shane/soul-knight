#ifndef _KNIGHT_H_
#define _KNIGHT_H_
#include "cocos2d.h"
#include "Entity.h"

USING_NS_CC;

class Hall;
class BattleRoom;
class Weapon;
class Prop;
class Boss;
class Enemy;

class Knight : public Entity {
	friend class BattleScene;
	friend class SafeScene;
	static constexpr float moveSpeed = 5.8f;
	static constexpr INT32 ultimateSkillGap = 720;

public:
	Knight() = default;

	~Knight();

	CREATE_FUNC(Knight);

	virtual bool init();

	void registerKeyboardEvent();

	void weaponAttack(Vec2 last);

	void useUltimateSkill();

	void bindBattleRoom(BattleRoom*);

	void bindHall(Hall*);

	void bindWeapon(Weapon*);

	void setNeedCreateBox(bool);

	bool getNeedCreateBox();

	INT32 getMP() const; //法力值

	void setMP(INT32);

	INT32 getMaxMP() const; //最大生命值

	void setMaxMP(INT32);

	INT32 getArmor() const; //护甲

	void setArmor(INT32);

	INT32 getMaxArmor() const; //最大护甲

	void setMaxArmor(INT32);

	INT32 getDamageBuff() const; //伤害buff

	void setDamageBuff(INT32);

	INT32 getMoveSpeedBuff() const; //移速buff

	void setMoveSpeedBuff(INT32);

	void deductHP(INT32) override;  //扣血

	void resumeArmor();             //恢复护甲

	bool checkStatue(); //检测雕像

	bool checkPortal(); //检测传送门

	BattleRoom* getAtBattleRoom();

	Hall* getAtHall();

	Animate* getAnimate();

	Sprite* collisionWithBoxCheck();

	Weapon* collisionWithWeaponCheck();

	Prop* collisionWithCropCheck();

	Enemy* collisionWithEnemyCheck();

	Boss* collisionWithBossCheck();

	void addGold(INT32);

private:
	INT32 armor = 5, maxArmor = 5;
	INT32 MP = 200, maxMP = 200;
	INT32 damageBuff = 1, moveSpeedBuff = 0;
	INT32 gold = 0;
	time_t preAttackedTime = 0, curTime = 0;
	time_t ultimateSkillTime = ultimateSkillGap;
	Weapon* weapon = nullptr;
	bool needCreateBox;
	BattleRoom* atBattleRoom = nullptr; //目前在哪个房间
	Hall* atHall = nullptr;
	bool isInvincible = false; //是否无敌
	bool goIntoPortal = false;
	bool isAnimation = false;//是否在执行帧动画动作
	int attackCount;
};

#endif
