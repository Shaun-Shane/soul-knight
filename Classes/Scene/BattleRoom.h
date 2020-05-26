#ifndef _BATTLEROOM_H
#define _BATTLEROOM_H

#include "cocos2d.h"
#include "Enemy\EnemyController.h"
#include "Const\Const.h"
USING_NS_CC;

class BattleRoom : public Node {
  static constexpr INT32 SIZEROOM = 17;
  friend class BattleScene;
 public:
  CREATE_FUNC(BattleRoom);

  virtual bool init();
  //virtual void update(float delta);
  float getCenterX();
  float getCenterY();
  INT32 getX();
  INT32 getY();

  void createMap(); //生成该房间地图

  void setCenter(float, float); // 设置中心

  void generateFloor(float, float);

  void generateWall(float, float);

  void generateDoor(float, float);

  void addDoor();

  void connect(BattleRoom*);

 private:
  float centerX, centerY;
  float upLeftX, upLeftY;
  float downRightX, downRightY;

  INT32 x, y;  // row and column in SIZEMTX * SIZEMTX BattleRoom matrix
  INT32 sizeHeight, sizeWidth;  // cnt of sprites in Height and Width

  bool visDir[CNTDIR] = {false};

 private:
  EnemyController* enemyCtr = nullptr;

 private:
  Vector<Sprite*> vecFloor; //open door can be considered as floor
  Vector<Sprite*> vecWall;

  Vector<Sprite*> vecDoorOpen, vecDoorClose; 
};

#endif
