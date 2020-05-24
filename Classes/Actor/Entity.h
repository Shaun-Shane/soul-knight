#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "cocos2d.h"

USING_NS_CC;

class Entity : public Node {
public : 
  Entity();
  Entity(INT32, INT32, float);
  Entity(Sprite* sprite);

  Sprite* getSprite();
  void bindSprite(Sprite* sprite);

  INT32 getHP(); // return HP of this entity
  void deductHP(INT32); //¿ÛÑª

  ~Entity();

private:
  Sprite* sprite;
protected:
  INT32 attack = 3;
  INT32 HP = 5;
  float attackSpeed;  //¹¥»÷ËÙ¶È
};

#endif;
