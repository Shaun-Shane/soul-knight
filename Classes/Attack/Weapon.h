#ifndef _WEAPON_H_
#define _WEAPON_H_
#include "cocos2d.h"
#include "Bullet.h"
#include "Scene/BattleRoom.h"
#include "Scene/Hall.h"
#include "Actor/Knight.h"

USING_NS_CC;

class Weapon : public Entity2 {
public:
	Weapon();

	CREATE_FUNC(Weapon);

	virtual bool init();

	~Weapon();

	Bullet* createBullet(Vec2 speed, INT32 firePower, bool);

	void setFireSpeed(float fireSpeed);

	float getFireSpeed();

	void setAttack(INT32 attack);

	INT32 getAttack();

	INT32 getMPConsumption();

	void setMPConsumption(INT32);

	bool getWeaponState();

	void setWeaponState(bool state);

	void knifeAttack(Knight* knight);

	void setBulletType(INT32);

	void weaponInit(float speed, INT32 attack, INT32 decMP, int weaponType, bool state, int bulletType);
protected:
	float fireSpeed;
	INT32 attack;
	INT32 mpConsumption;
	bool weaponState;
	INT32 bulletType;
};
#endif;
