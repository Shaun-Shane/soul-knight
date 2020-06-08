#ifndef _HALL_H_
#define _HALL_H_

#include "cocos2d.h"
#include "Const/Const.h"
#include "Actor/Knight.h"
#include "Attack/Bullet.h"
USING_NS_CC;

class Hall : public Node{
  friend class BattleScene;

 public:
  CREATE_FUNC(Hall);

  virtual bool init();
  
  virtual void update(float delta);

  virtual void createMap();  //生成走廊地图

  virtual void changePositionBy(float, float);

  virtual bool checkPlayerPosition(Knight*, float&, float&);

  void generateFloor(float, float, INT32);

  void generateWall(float, float, INT32);

  Vector<Bullet*>& getVecPlayerBullet(); 

  bool isInScreen(Vec2 pos);

  void bulletMove();


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

};

#endif  // _HALL_H_
