#include "SafeScene.h"

Scene* SafeScene::createScene() { return SafeScene::create(); }

void SafeScene::update(float delta) { 
  float ispeedX = knight->moveSpeedX;
  float ispeedY = knight->moveSpeedY;

  float curX = knight->getPositionX(), curY = knight->getPositionY();
  knight->setPosition(Point(curX + ispeedX, curY + ispeedY));
}

bool SafeScene::init() {
  if (!Scene::init()) {
    return false;
  }

  Size visibleSize = Director::getInstance()->getVisibleSize();

  // add knight to scene
  this->knight = Knight::create();
  this->knight->bindSprite(Sprite::create("Character//Knight.png"), 10);

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
