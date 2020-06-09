﻿#include "SafeScene.h"
#include"SetScene.h"
#include"BattleScene.h"
#include"StartScene.h"

Scene* SafeScene::createScene() { return SafeScene::create(); }

void SafeScene::update(float delta) { 
	float ispeedX = knight->moveSpeedX;
	float ispeedY = knight->moveSpeedY;

	float curX = knight->getPositionX(), curY = knight->getPositionY();
	knight->setPosition(Point(curX + ispeedX, curY + ispeedY));

	/*进度条更新*/
	BloodLoadingBar->setPercent(this->knight->getHP() * 100 / 5);
	ArmorLoadingBar->setPercent(this->knight->getArmor() * 100 / 5);
	MPLoadingBar->setPercent(float(this->knight->getMP()) / 200.0f * 100);

	/*状态信息更新*/
	HPLabel->setString(Value(this->knight->HP).asString() + "/5");
	armorLabel->setString(Value(this->knight->armor).asString() + "/5");
	MPLabel->setString(Value(this->knight->MP).asString() + "/200");

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

  /*创建状态信息进度条*/
  auto StatusBackGround = Sprite::create("Character//StatusBackground.png");

  BloodLoadingBar = ui::LoadingBar::create("Character//StatusBlood.png");
  ArmorLoadingBar = ui::LoadingBar::create("Character//StatusArmor.png");
  MPLoadingBar = ui::LoadingBar::create("Character//StatusMP.png");

  BloodLoadingBar->setDirection(ui::LoadingBar::Direction::RIGHT);
  ArmorLoadingBar->setDirection(ui::LoadingBar::Direction::RIGHT);
  MPLoadingBar->setDirection(ui::LoadingBar::Direction::RIGHT);

  StatusBackGround->setPosition(80, 680);
  BloodLoadingBar->setPosition(Vec2(89, 698));
  ArmorLoadingBar->setPosition(Vec2(89, 680));
  MPLoadingBar->setPosition(Vec2(89, 664));

  this->addChild(StatusBackGround, TOP);
  this->addChild(BloodLoadingBar, TOP);
  this->addChild(ArmorLoadingBar, TOP);
  this->addChild(MPLoadingBar, TOP);

  /*状态数字信息*/
  HPLabel=Label::createWithTTF("0", "fonts/Marker Felt.ttf",15);
  armorLabel=Label::createWithTTF("0", "fonts/Marker Felt.ttf", 15);
  MPLabel=Label::createWithTTF("0", "fonts/Marker Felt.ttf", 15);

  HPLabel->setPosition(Vec2(89, 698));
  armorLabel->setPosition(Vec2(89, 680));
  MPLabel->setPosition(Vec2(89, 664));

  this->addChild(HPLabel, TOP);
  this->addChild(armorLabel, TOP);
  this->addChild(MPLabel, TOP);

  // add knight to scene
  this->knight = Knight::create();
  this->knight->bindSprite(Sprite::create("Character//Knight1.png"), LayerPlayer);

  this->knight->setPosition(Point(visibleSize.width / 2, 40));
  this->addChild(this->knight);

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
