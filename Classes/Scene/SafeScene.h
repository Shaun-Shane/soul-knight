#ifndef _SAfESCENE_H
#define _SAfESCENE_H
#include "cocos2d.h"

#include "Actor\Knight.h"

USING_NS_CC;

class SafeScene : public Scene {
 public:
  static Scene* createScene();
  virtual bool init();
  virtual void update(float delta);
  CREATE_FUNC(SafeScene);

 private:
  void initBG();

 private:
  Sprite* backGround = nullptr;
  Sprite* portal = nullptr;

  Knight* knight = nullptr;
};

#endif
