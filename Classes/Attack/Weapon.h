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

    void bindWeapon(Weapon* weapon);
    Bullet* createBullet(Vec2 speed,Vec2 curPos, BattleRoom*, Hall*);  //to do再添加两个参数 atBattleRoom和atHall
    float getSpeed();
    ~Weapon();
private:
    float speed;

};

#endif;
