#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "cocos2d.h"

USING_NS_CC;

class Entity : public Node {
public : 
  Entity();
  Entity(INT32, INT32, float, float, float);
  Entity(Sprite* sprite);

  Sprite* getSprite();
  void bindSprite(Sprite* sprite, INT32 layer);
  void addShadow(const Point&, INT32);  //添加影子

  INT32 getHP(); // return HP of this entity
  void deductHP(INT32); //扣血
  void setHP(INT32 HP);
  float getMoveSpeedX();
  float getMoveSpeedY();

  ~Entity();

private:
  Sprite* sprite;
protected:
  INT32 attack = 3;
  INT32 HP = 5;

  float attackSpeed;  //攻击速度
  float moveSpeedX;  //x方向移动速度
  float moveSpeedY; //y方向移动速度
};

#endif;
