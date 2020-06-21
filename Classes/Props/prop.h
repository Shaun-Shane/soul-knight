#ifndef _PROP_H_
#define _PROP_H_
#include "cocos2d.h"
#include "Attack/Entity2.h"
#include "Actor/Knight.h"
USING_NS_CC;

class Prop : public Entity2 {
public:
	Prop();

	~Prop();

	CREATE_FUNC(Prop);

	virtual bool init();

	INT32 getPropIndex();

	void setPropIndex(INT32 index);

	void useProps(Knight*);

protected:
	INT32 propIndex;
};

#endif;
