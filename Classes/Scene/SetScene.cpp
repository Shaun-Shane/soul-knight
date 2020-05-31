#include"SetScene.h"
#include"StartScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* SetScene::createScene()
{
	return SetScene::create();
}


static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool SetScene::init()
{

	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/*创建菜单*/
	auto bkMusicLab = Label::createWithTTF("Background Music : ", "fonts/Marker Felt.ttf", 72);

	/*ON，OFF菜单在点击时会切换*/
	auto changeLabOn = Label::createWithTTF("ON", "fonts/Marker Felt.ttf", 72);
	auto changeLabOff = Label::createWithTTF("OFF", "fonts/Marker Felt.ttf", 72);

	auto exitImg = MenuItemImage::create(
		"exit.png",
		"exit01.png",
		CC_CALLBACK_1(SetScene::menuCloseCallbackEnd, this));

	auto changeMenuOn = MenuItemLabel::create(changeLabOn, CC_CALLBACK_1(SetScene::menuCloseCallbackChange, this));
	auto changeMenuOff = MenuItemLabel::create(changeLabOff, CC_CALLBACK_1(SetScene::menuCloseCallbackChange, this));

	 Menu01 = Menu::create(changeMenuOn, NULL);
	 Menu02 = Menu::create(changeMenuOff, NULL);
	 Menu03 = Menu::create(exitImg, NULL);

	 /*设置坐标*/
	bkMusicLab->setPosition(600, visibleSize.height / 2);
	Menu01->setPosition(Point(950, visibleSize.height / 2));
	Menu02->setPosition(Point(950, visibleSize.height / 2));
	Menu03->setPosition(1200, 660);

	this->addChild(bkMusicLab, 1);
	this->addChild(Menu01, 1);
	this->addChild(Menu02, 1);
	this->addChild(Menu03, 1);

	Menu02->setVisible(false);

	return true;
}

/*关闭设置面板的回调*/
void SetScene::menuCloseCallbackEnd(Ref* pSender)
{
	Director::getInstance()->popScene();
}

/*改变背景音乐播放状态的回调*/
void SetScene::menuCloseCallbackChange(Ref* pSender)
{
	/*切换On，off菜单的显示状态*/
	Menu01->setVisible(1-(Menu01->isVisible()));
	Menu02->setVisible(1-(Menu02->isVisible()));

	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();

	/*切换音乐的播放状态*/
	if (Menu01->isVisible()) {
		audio->resumeBackgroundMusic();
	}
	else {
		audio->stopBackgroundMusic();
	}
}


