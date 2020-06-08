#include "SafeScene.h"
#include"SetScene.h"
#include"BattleScene.h"
#include"StartScene.h"

Scene* SafeScene::createScene() { return SafeScene::create(); }

void SafeScene::update(float delta) { 
	float ispeedX = knight->moveSpeedX;
	float ispeedY = knight->moveSpeedY;

	float curX = knight->getPositionX(), curY = knight->getPositionY();
	knight->setPosition(Point(curX + ispeedX, curY + ispeedY));

	/*若位于传送门处进入战斗界面*/
	if (isInDoor()) {
		Director::getInstance()->pushScene(TransitionFade::create(1.0f, BattleScene::createScene()));
	}
}

bool SafeScene::init() {
  if (!Scene::init()) {
    return false;
  }

  Size visibleSize = Director::getInstance()->getVisibleSize();

  /*创建关闭按钮以及设置按钮*/
  auto setImg = MenuItemImage::create(
	  "set.png",
	  "set.png",
	  CC_CALLBACK_1(SafeScene::menuCloseCallbackSet, this));
  auto exitImg = MenuItemImage::create(
	  "exit.png",
	  "exit01.png",
	  CC_CALLBACK_1(SafeScene::menuCloseCallbackEnd, this));

  auto Menu01 = Menu::create(setImg, NULL);
  auto Menu02 = Menu::create(exitImg, NULL);

  Menu01->setPosition(1060, 660);
  Menu02->setPosition(1200, 660);

  this->addChild(Menu01, 1);
  this->addChild(Menu02, 1);

  // add knight to scene
  this->knight = Knight::create();
  this->knight->bindSprite(Sprite::create("Character//Knight1.png"), 10);

  this->knight->setPosition(Point(visibleSize.width / 2, 40));
  this->addChild(this->knight, 10);

  this->knight->setScaleX(0.3f);
  this->knight->setScaleY(0.3f);

  this->portal = Sprite::create("Map//portal3.png");
  this->addChild(portal, 1);
  this->portal->setPosition(Point(visibleSize.width / 2 + 50, visibleSize.height - 80));

  initBG();

  this->scheduleUpdate();

  return true;
}

void SafeScene::initBG() {
  Size visibleSize = Director::getInstance()->getVisibleSize();
  
  //background sprite1
  this->backGround = Sprite::create("Scene//SafeScene.png");

  Size bgSize1 = this->backGround->boundingBox().size;
  this->backGround->setScale(visibleSize.width / bgSize1.width,
	  visibleSize.height / bgSize1.height);

  this->backGround->setPosition(
	  Point(visibleSize.width / 2, visibleSize.height / 2));

  this->addChild(this->backGround, 0);
}

/*退出游戏*/
void SafeScene::menuCloseCallbackEnd(Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, StartScene::createScene()));
}

/*进入设置面板*/
void SafeScene::menuCloseCallbackSet(Ref* pSender)
{
	CCScheduler* defaultScheduler = CCDirector::sharedDirector()->getScheduler();
	defaultScheduler->pauseTarget(this);
	Director::getInstance()->pushScene(TransitionFade::create(3.0f, SetScene::createScene()));
	defaultScheduler->resumeTarget(this);
}

/*判断是否位于传送门处*/
bool SafeScene::isInDoor() {
	float CurX = knight->getPositionX(), CurY = knight->getPositionY();

	/*位置界定*/
	if (CurX >= 565 && CurX <= 715 && CurY >= 640 && CurY <= 720) {
		return 1;
	}
	else {
		return 0;
	}
}
