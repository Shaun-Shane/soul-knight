#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "Actor\Entity.h"
#include "Actor\Knight.h"

class Enemy : public Entity{
 public:
  Enemy();
  ~Enemy();
  CREATE_FUNC(Enemy);
  virtual bool init();

 public:
  void show();
  void hide();
  void reset();
  bool isAlive();
  bool isCollideWithKnight(Knight* knight);

 private:
  bool enemyIsAlive;
};

#endif