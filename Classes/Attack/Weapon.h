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

  void bindWeapon(Weapon* weapon);
  Bullet* createBullet(Vec2 speed,INT32 firePower); 
  void setFireSpeed(float fireSpeed);
  float getFireSpeed();

  
protected:
  float fireSpeed;

};

#endif;
