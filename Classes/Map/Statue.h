#ifndef _STATUE_H_
#define _STATUE_H

#include "cocos2d.h"
#include "Actor/Knight.h"
#include "Const/Const.h"

USING_NS_CC;

class Statue : public Node {
	static constexpr INT32 CNT_TYPE = 9;
public:
	Statue() = default;

	CREATE_FUNC(Statue);

	virtual bool init();

	void bindSprite(Sprite*, INT32);

	void setGoldCost(INT32);

	INT32 getGoldCost() const; //金币花费

	INT32 getStatueType() const; //雕像类型

	void setPlayerVisited(bool);

	bool getPlayerVisited() const;

	void giveBuff(Knight*);

	Label* getTextLabel() const;

private:
	Label* textLabel = nullptr;

private:
	INT32 statueType = 0;
	INT32 goldCost = 0;
	bool playerVisited = false;
};

#endif
