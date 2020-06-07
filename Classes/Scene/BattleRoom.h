#ifndef _BATTLEROOM_H
#define _BATTLEROOM_H

#include "cocos2d.h"
#include "Hall.h"
#include "Enemy\Enemy.h"
#include "Actor\Knight.h"
#include "Const\Const.h"
#include "Attack/Weapon.h"
#include "Props/prop.h"
USING_NS_CC;

class BattleRoom : public Hall {
  friend class BattleScene;
  static void createRoom(BattleRoom*&, BattleRoom*, INT32, INT32, INT32);

 public:
  CREATE_FUNC(BattleRoom);

  virtual bool init();

  virtual void update(float delta);

  void createMap() override; //生成该房间地图

  bool checkPlayerPosition(Knight*, float&, float&) override;

  void setCenter(float, float); // 设置中心

  void generateDoor(float, float, INT32 layer);

  void closeDoor();

  void openDoor();

  Vector<Enemy*>& getVecEnemy();

  Vector<Sprite*>& getVecEnemyBullet(); //Sprite改为子弹类名

  Vector<Prop*>& getVecProps();

  void playerBulletCollistionCheck();

  bool allKilled();
  
  void createTreasureBox();

  void crearteWeapon(int randomDigit);

  void createProps(int randomDigit);
 private:
  void createEnemy();  //生成敌人

 private:
  float centerX, centerY;

  INT32 x, y;  // row and column in SIZEMTX * SIZEMTX BattleRoom matrix
  INT32 roomType = NORMAL;

  bool visDir[CNTDIR] = {false};

  bool playerVisited = false;

 private:
  Sprite* portal = nullptr; //传送门

 private:
  Vector<Sprite*> vecDoorOpen, vecDoorClose; 

  Vector<Enemy*> vecEnemy;


  Vector<Sprite*> vecEnemyBullet;
  // 储存场景敌人子弹 基类中Hall有玩家的子弹Vector Sprite改为子弹类名

  Vector<Prop*> vecProps;

};

#endif
