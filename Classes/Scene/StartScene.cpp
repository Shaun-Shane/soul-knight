#include "StartScene.h"
#include"Scene/SafeScene.h"
#include"SetScene.h"

USING_NS_CC;

Scene* StartScene::createScene() { return StartScene::create(); }

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool StartScene::init()
{

	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/*背景音乐*/
	audio = CocosDenshion::SimpleAudioEngine::getInstance();

	if (!audio->isBackgroundMusicPlaying())
		audio->playBackgroundMusic("bkMusic.mp3", true);

	/*开始界面背景图像*/
	auto startImg = Sprite::create("start.png");
	if (startImg == nullptr)
	{
		problemLoading("'start.png'");
	}
	else {
		startImg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		startImg->setScale(1.295f);
		this->addChild(startImg, 0);
	}

	/*创建菜单*/
	auto startLab = Label::createWithTTF("START", "fonts/Marker Felt.ttf", 72);
	auto exitLab = Label::createWithTTF("EXIT", "fonts/Marker Felt.ttf", 72);

	auto startMenu = MenuItemLabel::create(startLab, CC_CALLBACK_1(StartScene::menuCloseCallbackStart, this));
	auto exitMenu = MenuItemLabel::create(exitLab, CC_CALLBACK_1(StartScene::menuCloseCallbackEnd, this));

	auto setImg = MenuItemImage::create(
		"set.png",
		"set.png",
		CC_CALLBACK_1(StartScene::menuCloseCallbackSet, this));
	auto exitImg = MenuItemImage::create(
		"exit.png",
		"exit01.png",
		CC_CALLBACK_1(StartScene::menuCloseCallbackEnd, this));

	auto Menu01 = Menu::create(startMenu, exitMenu, NULL);
	auto Menu02 = Menu::create(setImg, NULL);
	auto Menu03 = Menu::create(exitImg, NULL);

	/*垂直排列*/
	Menu01->alignItemsVertically();

	Menu01->setPosition(Point(visibleSize.width / 2, visibleSize.height / 4));
	Menu02->setPosition(1060, 660);
	Menu03->setPosition(1200, 660);

	this->addChild(Menu01, 1);
	this->addChild(Menu02, 1);
	this->addChild(Menu03, 1);

	return true;
}

/*退出游戏*/
void StartScene::menuCloseCallbackEnd(Ref* pSender)
{
	Director::getInstance()->end();
}

/*开始游戏*/
void StartScene::menuCloseCallbackStart(Ref* pSender)
{
	Director::getInstance()->pushScene(TransitionCrossFade::create(2.0f, SafeScene::createScene()));
}

/*进入设置面板*/
void StartScene::menuCloseCallbackSet(Ref* pSender)
{
	CCScheduler* defaultScheduler = CCDirector::sharedDirector()->getScheduler();
	defaultScheduler->pauseTarget(this);
	Director::getInstance()->pushScene(TransitionFade::create(3.0f, SetScene::createScene()));
	defaultScheduler->resumeTarget(this);
}


