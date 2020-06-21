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

	void menuCloseCallbackVolumeHigher(cocos2d::Ref* pSender);//升高音量
	void menuCloseCallbackVolumeLower(cocos2d::Ref* pSender);//降低音量

	CREATE_FUNC(SetScene);
private:
	//菜单
	cocos2d::Menu* Menu01;
	cocos2d::Menu* Menu02;
	cocos2d::Menu* Menu03;
	cocos2d::Menu* MenuHigherVolume;
	cocos2d::Menu* MenuLowerVolume;

	/*音量大小标签*/
	cocos2d::Label* volumeNumLab;

	int volume = 50;//初始音量
};

#endif 
