/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"
#include"SetScene.h"
#include"StartScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

Scene* HelloWorld::createScene() { return HelloWorld::create(); }

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename) {
  printf("Error while loading: %s\n", filename);
  printf(
      "Depending on how you compiled you might have to add 'Resources/' in "
      "front of filenames in HelloWorldScene.cpp\n");
}

void HelloWorld::backHome() {
  Size visibleSize = Director::getInstance()->getVisibleSize();

  auto label = Label::create("I am Home!", "Arial", 35);

  label->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
  this->addChild(label);
  //
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
  if (!Scene::init()) {
    return false;
  }

  /*创建关闭按钮以及设置按钮*/
  auto setImg = MenuItemImage::create(
	  "set.png",
	  "set.png",
	  CC_CALLBACK_1(HelloWorld::menuCloseCallbackSet, this));
  auto exitImg = MenuItemImage::create(
	  "exit.png",
	  "exit01.png",
	  CC_CALLBACK_1(HelloWorld::menuCloseCallbackEnd, this));

  auto Menu01 = Menu::create(setImg, NULL);
  auto Menu02 = Menu::create(exitImg, NULL);

  Menu01->setPosition(1060, 660);
  Menu02->setPosition(1200, 660);

  this->addChild(Menu01, 1);
  this->addChild(Menu02, 1);

//Original helloWorld init code is at bottom, just change ifndef to ifdef
#ifndef Test_One_By_One_Touch
  Size visibleSize = Director::getInstance()->getVisibleSize();

  auto sp1 = Sprite::create("Character//wizard.png");
  sp1->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
  this->addChild(sp1);

  auto sp2 = Sprite::create("Character//Knight.png");
  sp2->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
  this->addChild(sp2);

  auto listener = EventListenerTouchOneByOne::create();
  listener->onTouchBegan = [](Touch* touch, Event* event) {
    auto target = static_cast<Sprite*>(event->getCurrentTarget());

    Point pos =
        Director::getInstance()->convertToGL(touch->getLocationInView());

    if (target->getBoundingBox().containsPoint(pos)) {
      target->setOpacity(100);
      return true;
    }
    return false;
    //
  };

  // AllAtOnce->onTouchesEnded
  listener->onTouchEnded = [](Touch* touch, Event* event) {
    auto target = static_cast<Sprite*>(event->getCurrentTarget());

    target->setOpacity(255);
  };

  listener->setSwallowTouches(true);

  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, sp1);

  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(),
                                                           sp2);
#endif

#ifdef Test_touch
  auto listener = EventListenerTouchOneByOne::create();
  listener->onTouchBegan = [](Touch* touch, Event* event) {
    Point pos2 = touch->getLocationInView();
    Point pos3 = Director::getInstance()->convertToGL(pos3);

    log("touching x = %f, y = %f", pos3.x, pos3.y);
    return true;
  };

  listener->onTouchMoved = [](Touch* touch, Event* event) {
    log("on touch moved");
  };

  listener->onTouchEnded = [](Touch* touch, Event* event) {
    log("on touch ended");
  };

  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

#endif

#ifdef Test_lambda
  auto func = [&]() {
    auto wizard = Sprite::create("wizard.png");
    wizard->setPosition(Point(200, 200));
    this->addChild(wizard);
    wizard->setScale(2);
  };

  auto callFunc = CallFunc::create(func);
  this->runAction(callFunc);
#endif

#ifdef Test_Action_call_back
  Size visibleSize = Director::getInstance()->getVisibleSize();

  Sprite* wizard = Sprite::create("wizard.png");
  wizard->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
  this->addChild(wizard);

  auto moveToHome =
      MoveTo::create(10.0f, Point(visibleSize.width, visibleSize.height / 2));

  // lambda
  auto callbackFunc = [&]() { backHome(); };

  auto callFunc = CallFunc::create(callbackFunc);

  Action* actions = Sequence::create(moveToHome, callFunc, NULL);

  wizard->runAction(actions);
#endif

#ifdef Test_Seq_Spawn
  Size visibleSize = Director::getInstance()->getVisibleSize();

  Sprite* wizard = Sprite::create("wizard.png");
  wizard->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
  this->addChild(wizard);

  auto moveBy = MoveBy::create(2.2f, Point(40, 20));

  auto jumpBy = JumpBy::create(3.0f, Point(50, 1), 100, 5);

  auto rotateBy = RotateBy::create(2.5f, 200, 10);

  // Action* actions = Spawn::create(moveBy, jumpBy, rotateBy, NULL);
  Action* actions = Sequence::create(moveBy, jumpBy, rotateBy, NULL);
  wizard->runAction(actions);
#endif

#ifdef Test_repeat
  Size visibleSize = Director::getInstance()->getVisibleSize();

  Sprite* wizard = Sprite::create("wizard.png");
  wizard->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
  this->addChild(wizard);

  JumpBy* jumpBy = JumpBy::create(3.0f, Point(50, 1), 100, 1);

  RepeatForever* repeatForever = RepeatForever::create(jumpBy);

  Repeat* repeat = Repeat::create(jumpBy, 3);

  wizard->runAction(repeatForever);
#endif

#ifdef Test_blink
  Size visibleSize = Director::getInstance()->getVisibleSize();

  Sprite* wizard = Sprite::create("wizard.png");
  wizard->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
  this->addChild(wizard);

  Blink* blink = Blink::create(3.0f, 3);

  wizard->runAction(blink);
#endif

#ifdef Test_scale
  Size visibleSize = Director::getInstance()->getVisibleSize();

  Sprite* wizard = Sprite::create("wizard.png");
  wizard->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
  wizard->setScale(2.0f);
  this->addChild(wizard);

  ScaleBy* scaleTo = ScaleBy::create(2.8f, 0.4f, 1.0f);

  wizard->runAction(scaleTo);
#endif

#ifdef Test_move
  Size visibleSize = Director::getInstance()->getVisibleSize();

  Sprite* wizard = Sprite::create("wizard.png");
  wizard->setPosition(Point(50, visibleSize.height / 2));
  this->addChild(wizard);

  MoveBy* moveTo = MoveBy::create(0.9f, Point(250, 150));

  wizard->runAction(moveTo);
#endif

#ifdef Test_Scale9
  Scale9Sprite* wizard = Scale9Sprite::create("wizard.png");

  if (wizard == nullptr) {
    log("no such image");
    return true;
  }

  wizard->setContentSize(Size(50, 75));

  wizard->setPosition(Point(200, 200));

  this->addChild(wizard);
#endif

#ifdef Test_map
  Map<int, Label*> MapLabel;

  for (int i = 1; i <= 100; i++) {
    std::string name = "ShaunNo." + Value(i).asString();

    Label* lab = Label::create(name.c_str(), "Arial", 30);

    MapLabel.insert(i, lab);
  }

  Label* lab = MapLabel.at(1);

  lab->setPosition(Point(200, 400));

  this->addChild(lab);
#endif

#ifdef Test_vector
  Label* label1 = Label::create("Shaun", "Arial", 30);
  label1->setPosition(Point(250, 500));

  Label* label2 = Label::create("Shane", "Arial", 30);
  label2->setPosition(Point(250, 200));

  Vector<Label*> vec;

  vec.pushBack(label1), vec.pushBack(label2);

  for (auto lab : vec) {
    this->addChild(lab);
  }
#endif

#ifdef Test3_19
  auto valStr = Value("Shaun is");
  auto valInt = Value(0);
  log("%s %d", valStr.asString().c_str(), valInt.asInt());
#endif

#ifdef Test_3_14
  //��ȡ��Ļ��С
  Size visibleSize = Director::getInstance()->getVisibleSize();

  //�����˵���
  MenuItemImage* pCloseItem =
      MenuItemImage::create("CloseNormal.png", "CloseSelected.png", this,
                            menu_selector(HelloWorld::menuCloseCallback));

  //����һ����ǩ
  Label* label = Label::create("I am Label item.", "Arial", 30);

  //�ñ�ǩ���󴴽�һ����ǩ�˵��� created with label
  MenuItemLabel* pLabelItem = MenuItemLabel::create(label);

  //�����˵�
  Menu* pMenu = Menu::create(pCloseItem, pLabelItem, NULL);

  //���в˵����ڴ�ֱ�������Զ�����
  pMenu->alignItemsVertically();

  //���ò˵�����
  pMenu->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));

  //���ӵ��˵�����
  this->addChild(pMenu, 1);
#endif

#ifdef origin
  //////////////////////////////
  // 1. super init first
  if (!Scene::init()) {
    return false;
  }

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  /////////////////////////////
  // 2. add a menu item with "X" image, which is clicked to quit the program
  //    you may modify it.

  // add a "close" icon to exit the progress. it's an autorelease object
  auto closeItem =
      MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
                            CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

  if (closeItem == nullptr || closeItem->getContentSize().width <= 0 ||
      closeItem->getContentSize().height <= 0) {
    problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
  } else {
    float x =
        origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
    float y = origin.y + closeItem->getContentSize().height / 2;
    closeItem->setPosition(Vec2(x, y));
  }

  // create menu, it's an autorelease object
  auto menu = Menu::create(closeItem, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  /////////////////////////////
  // 3. add your codes below...

  // add a label shows "Hello World"
  // create and initialize a label

  auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
  if (label == nullptr) {
    problemLoading("'fonts/Marker Felt.ttf'");
  } else {
    // position the label on the center of the screen
    label->setPosition(
        Vec2(origin.x + visibleSize.width / 2,
             origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);
  }

  // add "HelloWorld" splash screen"
  auto sprite = Sprite::create("HelloWorld.png");
  if (sprite == nullptr) {
    problemLoading("'HelloWorld.png'");
  } else {
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x,
                             visibleSize.height / 2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
  }
#endif
  return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender) {
  // Close the cocos2d-x game scene and quit the application
  // Director::getInstance()->end();

  auto director = Director::getInstance();
  // director->replaceScene(TransitionSlideInT::create(3.0f,
  // secondScene::createScene()));
  director->pushScene(HelloWorld::createScene());

  /*To navigate back to native iOS screen(if present) without quitting the
   * application  ,do not use Director::getInstance()->end() as given
   * above,instead trigger a custom event created in RootViewController.mm as
   * below*/

  // EventCustom customEndEvent("game_scene_close_event");
  //_eventDispatcher->dispatchEvent(&customEndEvent);
}

/*退出游戏*/
void HelloWorld::menuCloseCallbackEnd(Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, StartScene::createScene()));
}

/*进入设置面板*/
void HelloWorld::menuCloseCallbackSet(Ref* pSender)
{
	CCScheduler* defaultScheduler = CCDirector::sharedDirector()->getScheduler();
	defaultScheduler->pauseTarget(this);
	Director::getInstance()->pushScene(TransitionFade::create(3.0f, SetScene::createScene()));
	defaultScheduler->resumeTarget(this);
}
