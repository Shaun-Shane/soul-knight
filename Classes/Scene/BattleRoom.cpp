#include "BattleRoom.h"

bool BattleRoom::init() {
  centerX = .0f, centerY = .0f;
  upLeftX = .0f, upLeftY = .0f;
  downRightX = .0f, downRightY = .0f;

  x = 0, y = 0;
  sizeHeight = SIZEROOM, sizeWidth = SIZEROOM;

  memset(visDir, false, sizeof(visDir));
  
  enemyCtr = EnemyController::create(); //controll enemy behaviors
  return true; 
}

void BattleRoom::setCenter(float X, float Y) { centerX = X, centerY = Y; }

float BattleRoom::getCenterX() { return centerX; }

float BattleRoom::getCenterY() { return centerY; }

INT32 BattleRoom::getX() { return x; }

INT32 BattleRoom::getY() { return y; }

void BattleRoom::generateFloor(float X, float Y) {
  INT32 randomNum = rand();
  Sprite* tmpSprite = nullptr;
  Value imageName("");

  if (randomNum % 6 == 0)
    imageName = "Map//floor3.png";
  else if (randomNum % 5 == 0)
    imageName = "Map//floor2.png";
  else imageName = "Map//floor1.png";

  tmpSprite = Sprite::create(imageName.asString().c_str());
  this->addChild(tmpSprite, 1);
  tmpSprite->setPosition(Point(X, Y));
  vecFloor.pushBack(tmpSprite);
}

void BattleRoom::generateWall(float X, float Y) {
  INT32 randomNum = rand();
  Sprite* tmpSprite = nullptr;
  Value imageName("");

  if (randomNum % 6 == 0)
    imageName = "Map//wall2.png";
  else
    imageName = "Map//wall1.png";

  tmpSprite = Sprite::create(imageName.asString().c_str());
  this->addChild(tmpSprite, 2);
  tmpSprite->setPosition(Point(X, Y + (WALLHEIGHT - FLOORHEIGHT) / 2));
  vecWall.pushBack(tmpSprite);
}

void BattleRoom::generateDoor(float X, float Y) {
  Sprite* tmpSprite = Sprite::create("Map//doorOpen.png");
  this->addChild(tmpSprite, 2);
  vecDoorOpen.pushBack(tmpSprite);

  tmpSprite->setPosition(Point(X, Y));
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

            ((W == sizeWidth - 1) && visDir[RIGHT] && (sizeHeight / 2 - 2 <= H) &&
             (H <= sizeHeight / 2 - 2 + SIZEHALL - 3)) ||

            ((H == 0) && visDir[DOWN] && (sizeWidth / 2 - 2 <= W) &&
             (W <= sizeWidth / 2 - 2 + SIZEHALL - 3)) ||

            ((H == sizeHeight - 1) && visDir[UP] && (sizeWidth / 2 - 2 <= W) &&
             (W <= sizeWidth / 2 - 2 + SIZEHALL - 3))) {
          log("%d %d", H, W);
          generateDoor(curX, curY);
        } else generateWall(curX, curY);
      } else
        generateFloor(curX, curY);

      // randomly generate floor and Wall

      curX += FLOORWIDTH;
    }
    curX = X, curY -= FLOORHEIGHT;
  }
}

void BattleRoom::addDoor() {
  for (INT32 dir = 0; dir < CNTDIR; dir++) {
    if (visDir[dir] == false) continue;
    log("st dir %d", dir);
    if (dir % 2 == 1) {
      const float X = centerX - FLOORWIDTH * (SIZEHALL / 2 - 1);
      const float Y = centerY + DIRY[dir] * FLOORHEIGHT * (sizeHeight / 2);
      
      float curX = X;
      for (INT32 W = 0; W < SIZEHALL - 2; W++) {
        generateDoor(curX, Y);
        curX += FLOORWIDTH;
      }
    }
    else {
      const float X = centerX + DIRX[dir] * FLOORWIDTH * (sizeWidth / 2);
      const float Y = centerY + FLOORHEIGHT * (SIZEHALL / 2 - 1);

      float curY = Y;
      for (INT32 H = SIZEHALL - 3; H >=0; H--) {
        generateDoor(X, curY);
        curY -= FLOORHEIGHT;
      }
    }
  }
}

