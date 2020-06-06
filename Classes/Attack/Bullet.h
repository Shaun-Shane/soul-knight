#ifndef _BULLET_H_
#define _BULLET_H_
#include "cocos2d.h"
#include "Entity2.h"
USING_NS_CC;

class Bullet : public Entity2 {
public:
<<<<<<< Updated upstream
    Bullet();
    ~Bullet();
    CREATE_FUNC(Bullet);
    virtual bool init();
    bool isInScreen();

    void setSpeed(Vec2 speed);
    Vec2 getSpeed();

    
private:
    Vec2 speed;
protected:
=======
  Bullet();
  ~Bullet();
  CREATE_FUNC(Bullet);
  virtual bool init();

  bool isInScreen();
  Vec2 getBulletSpeed();
  void setBulletSpeed(Vec2 speed);


protected:
  Vec2 bulletSpeed;

>>>>>>> Stashed changes
};

#endif;
