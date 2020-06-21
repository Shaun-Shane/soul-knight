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

protected:
	Sprite* sprite;

};
#endif;
