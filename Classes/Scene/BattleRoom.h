#ifndef _BATTLEROOM_H
#define _BATTLEROOM_H

#include "cocos2d.h"
#include "Enemy\EnemyController.h"
#include "Const\Const.h"
USING_NS_CC;

class BattleRoom : public Node {
  static constexpr INT32 SIZEROOM = 19;
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

  void connect(BattleRoom*);

 private:
  float centerX, centerY;
  INT32 x, y;  // row and column in SIZEMTX * SIZEMTX BattleRoom matrix
  INT32 visTime; //maybe not needed
  INT32 nextDIR;
  bool visDir[CNTDIR] = {false};

 private:
  BattleRoom* nextRoom = nullptr;
  BattleRoom* prevRoom = nullptr;

  EnemyController* enemyCtr = nullptr;

 private:
  Vector<Sprite*> vecFloor; //open door can be considered as floor
  Vector<Sprite*> vecWall;
  Vector<Sprite*> vecDoor;
};

#endif
