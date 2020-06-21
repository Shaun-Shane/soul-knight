#include"SetScene.h"
#include"StartScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* SetScene::createScene() { return SetScene::create(); }

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
	bkMusicLab->setPosition(600, 440);
	Menu01->setPosition(Point(950, 440));
	Menu02->setPosition(Point(950, 440));
	Menu03->setPosition(1200, 660);

	this->addChild(bkMusicLab, 1);
	this->addChild(Menu01, 1);
	this->addChild(Menu02, 1);
	this->addChild(Menu03, 1);

	/*音量控制按钮*/
	auto volumeLab = Label::createWithTTF("Volume : ", "fonts/Marker Felt.ttf", 72);
	volumeNumLab = Label::createWithTTF("100", "fonts/Marker Felt.ttf", 72);
	auto volumeHigherLab = Label::createWithTTF("+", "fonts/Marker Felt.ttf", 150);
	auto volumeLowerLab = Label::createWithTTF("-", "fonts/Marker Felt.ttf", 150);

	auto volumeHigherMenu = MenuItemLabel::create(volumeHigherLab, CC_CALLBACK_1(SetScene::menuCloseCallbackVolumeHigher, this));
	auto volumeLowerMenu = MenuItemLabel::create(volumeLowerLab, CC_CALLBACK_1(SetScene::menuCloseCallbackVolumeLower, this));

	MenuHigherVolume = Menu::create(volumeHigherMenu, NULL);
	MenuLowerVolume = Menu::create(volumeLowerMenu, NULL);

	volumeLab->setPosition(500, 260);
	volumeNumLab->setPosition(710, 260);
	MenuHigherVolume->setPosition(850, 250);
	MenuLowerVolume->setPosition(960, 260);

	this->addChild(volumeLab, 1);
	this->addChild(volumeNumLab, 1);
	this->addChild(MenuHigherVolume, 1);
	this->addChild(MenuLowerVolume, 1);

	/*修改音量数值*/
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();

	/*获取音量数值（将其转成10的倍数）*/
	INT32 tem = audio->getBackgroundMusicVolume() * 100 / 1;
	if (tem % 10 >= 5) {
		volume = tem / 10 * 10 + 10;
	}
	else {
		volume = tem / 10 * 10;
	}

	volumeNumLab->setString(Value(volume).asString());

	if (volume == 0) {
		MenuLowerVolume->setOpacity(100);
	}
	else if (volume == 100) {
		MenuHigherVolume->setOpacity(100);
	}

	/*修改ON/OFF状态*/
	if (audio->isBackgroundMusicPlaying()) {
		Menu01->setVisible(true);
		Menu02->setVisible(false);
	}
	else {
		Menu01->setVisible(false);
		Menu02->setVisible(true);
	}

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
	Menu01->setVisible(1 - (Menu01->isVisible()));
	Menu02->setVisible(1 - (Menu02->isVisible()));

	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();

	/*切换音乐的播放状态*/
	if (Menu01->isVisible()) {
		audio->resumeBackgroundMusic();
	}
	else {
		audio->stopBackgroundMusic();
	}
}

/*升高音量*/
void SetScene::menuCloseCallbackVolumeHigher(Ref* pSender) {
	/*检测是否达到最大值*/
	if (volume == 100) {
		return;
	}

	/*修改音量*/
	volume += 10;
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->setBackgroundMusicVolume(volume / 100.0f);

	/*标签变化*/
	volumeNumLab->setString(Value(volume).asString());
	MenuLowerVolume->setOpacity(255);
	if (volume == 100) {
		MenuHigherVolume->setOpacity(100);
	}
}

/*降低音量*/
void SetScene::menuCloseCallbackVolumeLower(Ref* pSender) {
	/*检测是否达到最大值*/
	if (volume == 0) {
		return;
	}

	/*修改音量*/
	volume -= 10;
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->setBackgroundMusicVolume(volume / 100.0f);

	/*标签变化*/
	volumeNumLab->setString(Value(volume).asString());
	MenuHigherVolume->setOpacity(255);
	if (volume == 0) {
		MenuLowerVolume->setOpacity(100);
	}
}


