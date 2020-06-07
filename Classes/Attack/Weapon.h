#ifndef _WEAPON_H_
#define _WEAPON_H_
#include "cocos2d.h"
#include "Bullet.h"
#include "Scene/BattleRoom.h"
#include "Scene/Hall.h"

USING_NS_CC;

class Weapon : public Entity2 {
public:
  Weapon();
  CREATE_FUNC(Weapon);
  virtual bool init();
  ~Weapon();

  Bullet* createBullet(Vec2 speed,INT32 firePower); 
  void setFireSpeed(float fireSpeed);
  float getFireSpeed();

  void setAttack(INT32 attack);
  INT32 getAttack();

  
protected:
  float fireSpeed;
  INT32 attack;

};

#endif;
