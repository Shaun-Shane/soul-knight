#ifndef _ENTITY2_H_
#define _ENTITY2_H_
#include "cocos2d.h"

USING_NS_CC;

class Entity2 : public Node {
public:
  Entity2();
  ~Entity2();

  Sprite* getSprite();
  void bindSprite(Sprite* sprite, INT32 layer);

  void setAttack(INT32 attack);
  INT32 getAttack();

protected:
  Sprite* sprite;
  INT32 attack;
};
#endif;
