#ifndef _HALL_H_
#define _HALL_H_

#include "cocos2d.h"
#include "Const/Const.h"
#include "Actor/Knight.h"
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

 private:
  INT32 dir; //direction of this hall
 protected:
  float upLeftX, upLeftY;
  float downRightX, downRightY;

  Knight* knight = nullptr;

  INT32 sizeHeight, sizeWidth;  // cnt of sprites in Height and Width
  
  Vector<Sprite*> vecPlayerBullet; //储存场景子弹 Sprite改为子弹类名

  Vector<Sprite*> vecFloor;
  Vector<Sprite*> vecWall;
};

#endif  // _HALL_H_
