#ifndef __START_SCENE__
#define __START_SCENE__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class StartScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	/*三个回调函数*/
	void menuCloseCallbackEnd(cocos2d::Ref* pSender);
	void menuCloseCallbackStart(cocos2d::Ref* pSender);
	void menuCloseCallbackSet(cocos2d::Ref* pSender);

	CREATE_FUNC(StartScene);
private:
	CocosDenshion::SimpleAudioEngine* audio;//背景音乐管理器
};

#endif 
