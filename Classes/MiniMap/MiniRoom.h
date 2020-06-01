#ifndef _MINIROOM_H_
#define _MINIROOM_H_
#include "cocos2d.h"
#include "Const/const.h"

USING_NS_CC;

class MiniRoom : public DrawNode {

public:
  CREATE_FUNC(MiniRoom);

  //virtual bool init();

 private:
  INT32 x, y;  // row and column in SIZEMTX * SIZEMTX MiniMap matrix

  DrawNode* miniHall[CNTDIR] = {nullptr};

  void setColorWhite() { 
     this->setColor(Color3B(245, 245, 245)); //烟白色
  }

  void setColorGrey() { 
     this->setColor(Color3B(128, 128, 128)); //灰色
     this->setColor(Color3B(105, 105, 105)); //深灰色
  }

  void setColorBlack() { 
     this->setColor(Color3B(0, 0, 0));//纯黑
  }

};

#endif