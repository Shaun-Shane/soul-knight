#ifndef _HALL_H_
#define _HALL_H_

#include "cocos2d.h"
#include "Const/Const.h"
USING_NS_CC;

class Hall : public Node{
  friend class BattleScene;

 public:
  CREATE_FUNC(Hall);

  virtual bool init();
  // virtual void update(float delta);

  virtual void createMap();  //生成走廊地图

  void generateFloor(float, float);

  void generateWall(float, float);

 private:
  INT32 dir; //direction of this hall
 protected:
  float upLeftX, upLeftY;
  float downRightX, downRightY;

  INT32 sizeHeight, sizeWidth;  // cnt of sprites in Height and Width

  Vector<Sprite*> vecFloor;
  Vector<Sprite*> vecWall;
};

#endif  // _HALL_H_
