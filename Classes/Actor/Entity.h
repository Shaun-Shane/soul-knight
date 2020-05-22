#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "cocos2d.h"

USING_NS_CC;

class Entity : public Node {
public : 
  Entity();
  Entity(Sprite* sprite);

  Sprite* GetSprite();
  void BindSprite(Sprite* sprite);

  ~Entity();

private:
  Sprite* sprite;
protected:
  INT32 attack = 3;
  INT32 armor = 5;
  INT32 HP = 5;
  INT32 MP = 200;
};

#endif;
