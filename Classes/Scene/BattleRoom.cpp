#include "BattleRoom.h"
#include"SetScene.h"

bool BattleRoom::init() {
	/*创建关闭按钮以及设置按钮*/
	auto setImg = MenuItemImage::create(
		"set.png",
		"set.png",
		CC_CALLBACK_1(BattleRoom::menuCloseCallbackSet, this));
	auto exitImg = MenuItemImage::create(
		"exit.png",
		"exit01.png",
		CC_CALLBACK_1(BattleRoom::menuCloseCallbackEnd, this));

	auto Menu01 = Menu::create(setImg, NULL);
	auto Menu02 = Menu::create(exitImg, NULL);

	Menu01->setPosition(1060, 660);
	Menu02->setPosition(1200, 660);

	this->addChild(Menu01, 1);
	this->addChild(Menu02, 1);

  centerX = .0f, centerY = .0f;
  upLeftX = .0f, upLeftY = .0f;
  downRightX = .0f, downRightY = .0f;
  x = 0, y = 0;
  sizeHeight = SIZEROOM, sizeWidth = SIZEROOM;

  memset(visDir, false, sizeof(visDir));

  enemyCtr = EnemyController::create();  // controll enemy behaviors
  portal = nullptr;
  return true;
}

void BattleRoom::setCenter(float X, float Y) { centerX = X, centerY = Y; }

void BattleRoom::generateDoor(float X, float Y, INT32 layer) {
  Sprite* tmpSprite = Sprite::create("Map//doorOpen.png");
  this->addChild(tmpSprite, LayerPlayer - 1);
  tmpSprite->setGlobalZOrder(LayerPlayer - 1);
  vecDoorOpen.pushBack(tmpSprite);

  tmpSprite->setPosition(Point(X, Y));
  tmpSprite->setVisible(true);

  tmpSprite = Sprite::create("Map//doorClose.png");
  this->addChild(tmpSprite, layer);
  tmpSprite->setGlobalZOrder(layer);
  vecDoorClose.pushBack(tmpSprite);

  tmpSprite->setPosition(Point(X, Y + FLOORHEIGHT / 2));
  tmpSprite->setVisible(false);  // closeDoor images are not visible at first
}

void BattleRoom::createMap() {
  srand(time(nullptr));

  const float X = centerX - FLOORWIDTH * (sizeWidth / 2);
  const float Y = centerY + FLOORHEIGHT * (sizeHeight / 2);
  //(X, Y) is upLeft Position;
  upLeftX = X + FLOORWIDTH, upLeftY = Y - FLOORHEIGHT;
  // get vertices Position
  downRightX = X + FLOORWIDTH * (sizeWidth - 2);
  downRightY = Y - FLOORHEIGHT * (sizeHeight - 2);

  float curX = X, curY = Y;
  for (INT32 H = sizeHeight - 1; H >= 0; H--) {  // for height and width
    for (INT32 W = 0; W <= sizeWidth - 1; W++) {
      if (H == 0 || H == sizeHeight - 1 || W == 0 || W == sizeWidth - 1) {
        if (((W == 0) && visDir[LEFT] && (sizeHeight / 2 - 2 <= H) &&
             (H <= sizeHeight / 2 - 2 + SIZEHALL - 3)) ||

            ((W == sizeWidth - 1) && visDir[RIGHT] &&
             (sizeHeight / 2 - 2 <= H) &&
             (H <= sizeHeight / 2 - 2 + SIZEHALL - 3)) ||

            ((H == 0) && visDir[DOWN] && (sizeWidth / 2 - 2 <= W) &&
             (W <= sizeWidth / 2 - 2 + SIZEHALL - 3)) ||

            ((H == sizeHeight - 1) && visDir[UP] && (sizeWidth / 2 - 2 <= W) &&
             (W <= sizeWidth / 2 - 2 + SIZEHALL - 3))) {
          if (H != sizeHeight - 1)
            generateDoor(curX, curY, LayerPlayer + 1);
          else
            generateDoor(curX, curY, LayerPlayer - 1);
        } else if (H != sizeHeight - 1 || W == 0 || W == sizeWidth - 1) {
          generateWall(curX, curY, LayerPlayer + 1);
        } else if (visDir[UP] && H == sizeHeight - 1 &&
                   (W == sizeWidth / 2 - 3 ||
                    W == sizeWidth / 2 + SIZEHALL - 4)) {
          generateWall(curX, curY, LayerPlayer + 2);
        } else {
          generateWall(curX, curY, LayerPlayer - 1);
        }
      } else {
        generateFloor(curX, curY, LayerPlayer - 2);
      }
      // randomly generate floor and Wall

      curX += FLOORWIDTH;
    }
    curX = X, curY -= FLOORHEIGHT;
  }
}

void BattleRoom::closeDoor() {  // doorClose sptires are visible
  for (auto sprite : vecDoorOpen) {
    sprite->setVisible(false);
  }

  for (auto sprite : vecDoorClose) {
    sprite->setVisible(true);
  }
}

void BattleRoom::openDoor() {  // doorOpen sptires are visible
  for (auto sprite : vecDoorOpen) {
    sprite->setVisible(true);
  }

  for (auto sprite : vecDoorClose) {
    sprite->setVisible(false);
  }
}

void BattleRoom::checkPlayerPosition(Knight* knight, float& ispeedX,
                                     float& ispeedY) {
  float knightX = knight->getPositionX();
  float knightY = knight->getPositionY();

  if (knightX >= upLeftX - FLOORWIDTH && knightX <= downRightX + FLOORWIDTH &&
      knightY <= upLeftY + FLOORHEIGHT && knightY >= downRightY - FLOORHEIGHT) {
    // log("%d %d %d %d", visDir[0], visDir[1], visDir[2], visDir[3]);
    if (enemyCtr->enemyAllKilled())
      openDoor();  //怪物全部击杀开门
    else
      closeDoor();

    if (enemyCtr->enemyAllKilled() == false) {
      if (ispeedX > 0 && knightX >= downRightX)
        ispeedX = .0f;
      else if (ispeedX < 0 && knightX <= upLeftX)
        ispeedX = .0f;
      else if (ispeedY > 0 && knightY >= upLeftY + 20)
        ispeedY = .0f;
      else if (ispeedY < 0 && knightY <= downRightY)
        ispeedY = .0f;
    } else {
      if (((upLeftY + FLOORHEIGHT / 2 - FLOORHEIGHT * (sizeHeight / 2 - 3)) >=
               knightY &&
           knightY >= (downRightY + FLOORHEIGHT * (sizeHeight / 2 - 3)))) {
        if (ispeedX > 0 && knightX >= downRightX && !visDir[RIGHT])
          ispeedX = .0f;
        if (ispeedX < 0 && knightX <= upLeftX && !visDir[LEFT]) ispeedX = 0.f;
      } else if (upLeftX + FLOORHEIGHT * (sizeHeight / 2 - 3) <= knightX &&
                 knightX <= downRightX - FLOORHEIGHT * (sizeHeight / 2 - 3)) {
        if (ispeedY > 0 && knightY >= upLeftY + FLOORHEIGHT / 2 && !visDir[UP]) ispeedY = .0f;
        if (ispeedY < 0 && knightY <= downRightY && !visDir[DOWN])
          ispeedY = 0.f;
      } else {
        if (ispeedX > 0 && knightX >= downRightX)
          ispeedX = .0f;
        else if (ispeedX < 0 && knightX <= upLeftX)
          ispeedX = .0f;
        else if (ispeedY > 0 && knightY >= upLeftY + 20)
          ispeedY = .0f;
        else if (ispeedY < 0 && knightY <= downRightY)
          ispeedY = .0f;
      }
    }
  }
}

/*退出游戏*/
void BattleRoom::menuCloseCallbackEnd(Ref* pSender)
{
	Director::getInstance()->end();
}

/*进入设置面板*/
void BattleRoom::menuCloseCallbackSet(Ref* pSender)
{
	CCScheduler* defaultScheduler = CCDirector::sharedDirector()->getScheduler();
	defaultScheduler->pauseTarget(this);
	Director::getInstance()->pushScene(TransitionFade::create(3.0f, SetScene::createScene()));
	defaultScheduler->resumeTarget(this);
}
