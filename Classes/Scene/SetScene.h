#ifndef __SET_SCENE__
#define __SET_SCENE__

#include "cocos2d.h"

class SetScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuCloseCallbackEnd(cocos2d::Ref* pSender);//关闭设置面板的回调函数
	void menuCloseCallbackChange(cocos2d::Ref* pSender);//改变音乐播放状态的回调函数

	CREATE_FUNC(SetScene);
private:
	//菜单
	cocos2d::Menu* Menu01;
	cocos2d::Menu* Menu02;
	cocos2d::Menu* Menu03;
};

#endif 
