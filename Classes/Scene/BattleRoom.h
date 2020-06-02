#ifndef _BATTLEROOM_H
#define _BATTLEROOM_H

#include "cocos2d.h"
#include "Hall.h"
#include "Enemy\EnemyController.h"
#include "Const\Const.h"
USING_NS_CC;

class BattleRoom : public Hall {
  friend class BattleScene;
  static void createRoom(BattleRoom*&, BattleRoom*, INT32, INT32, INT32);

 public:
  CREATE_FUNC(BattleRoom);

  virtual bool init();

  void createMap() override; //生成该房间地图

  void checkPlayerPosition(Knight*, float&, float&) override;

  void setCenter(float, float); // 设置中心

  void generateDoor(float, float, INT32 layer);

  void closeDoor();

  void openDoor();

 private:
  float centerX, centerY;

  INT32 x, y;  // row and column in SIZEMTX * SIZEMTX BattleRoom matrix

  bool visDir[CNTDIR] = {false};

 private:
  EnemyController* enemyCtr = nullptr;

  Sprite* portal = nullptr; //传送门

 private:
  Vector<Sprite*> vecDoorOpen, vecDoorClose; 
};

#endif
