#ifndef _BATTLESCENE_H_
#define _BATTLESCENE_H_

#include "Actor\Knight.h"
#include "Enemy\Enemy.h"
#include "BattleRoom.h"
#include "Hall.h"
#include "Const\Const.h"
#include "MiniMap\MiniMap.h"
#include "MiniMap\MiniRoom.h"
#include "cocos2d.h"
#include <queue>
#include"ui/CocosGUI.h"

USING_NS_CC;
using std::queue;

class BattleScene : public Scene {
  static constexpr INT32 SIZEMTX = 5;
  static constexpr INT32 MAXROOM = 6; // temporarily make it 6

 public:
  static Scene* createScene();
  virtual bool init();
  virtual void update(float delta);
  CREATE_FUNC(BattleScene);

  /*回调函数*/
  void menuCloseCallbackEnd(cocos2d::Ref* pSender);
  void menuCloseCallbackSet(cocos2d::Ref* pSender);

 private:
  void updatePlayerPos();

  void initRoom(); //generate room randomly
  void initMiniMap();

  void getToRoom(INT32, INT32, BattleRoom*, queue<BattleRoom*>&);
  void randomGenerate(INT32, INT32);

  void setHallWithWidth(Hall*, DrawNode*, const BattleRoom*, const BattleRoom*);
  void setHallWithHeight(Hall*, DrawNode*, const BattleRoom*, const BattleRoom*);
  void connectRoom(BattleRoom*);

 private:
  INT32 cntRoom = 0;
 private:
  Knight* knight = nullptr;
  MiniMap* miniMap = nullptr;
  
 // temporarily just one kind of map floor and wall
 private:
  Vector<Sprite*> vecFloor;
  Vector<Sprite*> vecWall;
  Vector<Sprite*> vecDoor;

  Vector<Enemy*> vecEnemy;
  Vector<Hall*> vecHall;

  BattleRoom* beginRoom = nullptr;
  BattleRoom* endRoom = nullptr;
  BattleRoom* battleRoom[SIZEMTX][SIZEMTX] = {nullptr};  // room

  ui::LoadingBar* BloodLoadingBar = ui::LoadingBar::create();
  ui::LoadingBar* ArmorLoadingBar = ui::LoadingBar::create();
  ui::LoadingBar* MPLoadingBar = ui::LoadingBar::create();
};

#endif
