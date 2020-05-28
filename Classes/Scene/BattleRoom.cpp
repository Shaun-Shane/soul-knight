#include "BattleRoom.h"

bool BattleRoom::init() {
  centerX = .0f, centerY = .0f;
  upLeftX = .0f, upLeftY = .0f;
  downRightX = .0f, downRightY = .0f;
  x = 0, y = 0;
  sizeHeight = SIZEROOM, sizeWidth = SIZEROOM;

  memset(visDir, false, sizeof(visDir));
  
  enemyCtr = EnemyController::create(); //controll enemy behaviors
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
  tmpSprite->setVisible(false); //closeDoor images are not visible at first
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
                   (W == sizeWidth / 2 - 3 || W == sizeWidth / 2 + SIZEHALL - 4)) {
          generateWall(curX, curY, LayerPlayer + 2);
        }
        else {
          generateWall(curX, curY, LayerPlayer - 1);
        }
      }
      else {
        generateFloor(curX, curY, LayerPlayer - 2);
      }
      // randomly generate floor and Wall

      curX += FLOORWIDTH;
    }
    curX = X, curY -= FLOORHEIGHT;
  }
}

void BattleRoom::closeDoor() { //doorClose sptires are visible
  for (auto sprite : vecDoorOpen) {
    sprite->setVisible(false);
  }

  for (auto sprite : vecDoorClose) {
    sprite->setVisible(true);
  }
}

void BattleRoom::openDoor() { //doorOpen sptires are visible
  for (auto sprite : vecDoorOpen) {
    sprite->setVisible(true);
  }

  for (auto sprite : vecDoorClose) {
    sprite->setVisible(false);
  }
}



