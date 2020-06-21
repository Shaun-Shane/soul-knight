#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "cocos2d.h"

USING_NS_CC;

class Entity : public Node {
public:
	Entity() = default;

	Sprite* getSprite();

	void bindSprite(Sprite* sprite, INT32 layer);

	void addShadow(const Point&, INT32);  //添加影子

	virtual void showDeathEffect();

	INT32 getHP() const; // return HP of this entity

	virtual void deductHP(INT32); //扣血

	void setHP(INT32 HP);

	INT32 getMaxHP() const;

	void setMaxHP(INT32);

	bool getIsKilled() const;

	void setIsKilled(bool);

	float getMoveSpeedX() const;

	void setMoveSpeedX(float);

	float getMoveSpeedY() const;

	void setMoveSpeedY(float);

	~Entity();

private:
	Sprite* sprite;
protected:
	INT32 attack = 3;
	INT32 HP = 5, maxHP = 5;

	float attackSpeed = 0.0f;  //攻击速度
	float moveSpeedX = 0.0f;  //x方向移动速度
	float moveSpeedY = 0.0f; //y方向移动速度

	bool isKilled = false;
};

#endif;