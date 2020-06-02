#ifndef _MINIROOM_H_
#define _MINIROOM_H_
#include "cocos2d.h"
#include "Const/const.h"

USING_NS_CC;

class MiniRoom : public DrawNode {
  friend class BattleScene;

 public:
  CREATE_FUNC(MiniRoom);

  void createRoom(float, float, Color4F);

 private:
  float downLeftX = .0f, downLeftY = .0f;  // DrawNode的顶点信息
  float upRightX = .0f, upRightY = .0f;

  INT32 x = 0, y = 0;  // row and column in SIZEMTX * SIZEMTX MiniMap matrix

  DrawNode* miniHall[CNTDIR] = {nullptr};
};

#endif