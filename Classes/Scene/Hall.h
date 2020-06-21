#ifndef _HALL_H_
#define _HALL_H_

#include "cocos2d.h"
#include "Const/Const.h"
#include "Actor/Knight.h"

USING_NS_CC;

class Bullet;

class Hall : public Node {
	friend class BattleScene;

public:
	CREATE_FUNC(Hall);

	virtual bool init();

	virtual void update(float delta);

	virtual void createMap();  //生成走廊地图

	virtual void changePositionBy(float, float);

	virtual bool checkPlayerPosition(Knight*, float&, float&);

	void generateFloor(float, float, INT32);

	Vector<Bullet*>& getVecEnemyBullet();

	void generateWall(float, float, INT32, bool);

	Vector<Bullet*>& getVecPlayerBullet();

	bool isInRange(Vec2 pos);

	Knight* getKnight();

	void bulletMove();

	Point getUpleftVertex() const;  //获得房间内左上角顶点坐标

	Point getDownRightVertex() const;  //获得房间内右下角顶点坐标

private:
	INT32 dir; //direction of this hall
protected:
	float upLeftX, upLeftY;

	float downRightX, downRightY;

	Knight* knight = nullptr;

	INT32 sizeHeight, sizeWidth;  // cnt of sprites in Height and Width

	Vector<Bullet*> vecPlayerBullet;

	Vector<Sprite*> vecFloor;

	Vector<Sprite*> vecWall;

	Vector<Bullet*> vecEnemyBullet;

};

#endif  // _HALL_H_
