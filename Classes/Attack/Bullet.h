#ifndef _BULLET_H_
#define _BULLET_H_
#include "cocos2d.h"
#include "Entity2.h"
#include "Scene/Hall.h"
USING_NS_CC;

class Bullet : public Entity2 {
public:
	Bullet();

	~Bullet();

	CREATE_FUNC(Bullet);

	virtual bool init();

	bool isInScreen();

	Vec2 getBulletSpeed();

	void setBulletSpeed(Vec2 speed);

	void setAttack(INT32 attack);

	void showEffect(Point, Hall*); //显示击中特效

	INT32 getAttack();

protected:
	Vec2 bulletSpeed;
	INT32 attack;
};
#endif;
