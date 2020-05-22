#ifndef _KNIGHT_H_
#define _KNIGHT_H_
#include "cocos2d.h"
#include "Entity.h"

USING_NS_CC;

class Knight : public Entity{
 public:
  Knight();
  ~Knight();
  CREATE_FUNC(Knight);
  virtual bool init();
};

#endif
