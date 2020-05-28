#ifndef _KNIGHT_H_
#define _KNIGHT_H_
#include "cocos2d.h"
#include "Entity.h"

USING_NS_CC;

class Knight : public Entity{
  friend class BattleScene;
  static constexpr float moveSpeed = 4.0f;

 public:
  Knight();
  ~Knight();
  CREATE_FUNC(Knight);
  virtual bool init();

  //virtual void update(float);

  void registerKeyboardEvent();

 private:
  INT32 armor;
  INT32 MP;
};

#endif
