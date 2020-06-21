#ifndef _SAfESCENE_H
#define _SAfESCENE_H
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "Actor\Knight.h"

USING_NS_CC;

class SafeScene : public Scene {
public:
	static Scene* createScene();

	virtual bool init();

	virtual void update(float delta);

	CREATE_FUNC(SafeScene);

	/*回调函数*/
	void menuCloseCallbackEnd(cocos2d::Ref* pSender);

	void menuCloseCallbackSet(cocos2d::Ref* pSender);

private:
	void initBG();

	/*判断角色是否进入传送门*/
	bool isInDoor();
private:
	Sprite* backGround = nullptr;
	Sprite* portal = nullptr;

	Knight* knight = nullptr;

	ui::LoadingBar* BloodLoadingBar = ui::LoadingBar::create();
	ui::LoadingBar* ArmorLoadingBar = ui::LoadingBar::create();
	ui::LoadingBar* MPLoadingBar = ui::LoadingBar::create();

	Label* HPLabel;
	Label* armorLabel;
	Label* MPLabel;
};

#endif
