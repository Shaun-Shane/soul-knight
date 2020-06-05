#include "Hall.h"

bool Hall::init() {
  upLeftX = .0f, upLeftY = .0f;
  downRightX = .0f, downRightY = .0f;

  sizeHeight = SIZEHALL, sizeWidth = SIZEHALL;

  this->scheduleUpdate();
  return true;
}

void Hall::update(float delta) {
  for (auto playerBullet : vecPlayerBullet) {
    if (playerBullet->getParent() == nullptr) continue;

    // if (...);
    //do something
  }
}

void Hall::generateFloor(float X, float Y, INT32 layer) {
  INT32 randomNum = rand();
  Sprite* tmpSprite = nullptr;
  Value imageName("");

  if (randomNum % 4 == 0)
    imageName = "Map//floor3.png";
  else if (randomNum % 3 == 0)
    imageName = "Map//floor2.png";
  else
    imageName = "Map//floor1.png";

  tmpSprite = Sprite::create(imageName.asString().c_str());
  this->addChild(tmpSprite);
  tmpSprite->setGlobalZOrder(layer);
  tmpSprite->setPosition(Point(X, Y));
  vecFloor.pushBack(tmpSprite);
}

void Hall::generateWall(float X, float Y, INT32 layer) {
  INT32 randomNum = rand();
  Sprite* tmpSprite = nullptr;
  Value imageName("");

  if (randomNum % 6 == 0)
    imageName = "Map//wall2.png";
  else
    imageName = "Map//wall1.png";

  tmpSprite = Sprite::create(imageName.asString().c_str(), Rect(0, 0, 40, 35));
  this->addChild(tmpSprite, layer);
  tmpSprite->setGlobalZOrder(layer);
  tmpSprite->setPosition(Point(X, Y + (WALLHEIGHT - FLOORHEIGHT)));
  vecWall.pushBack(tmpSprite);
  //Upside of whe wall

  tmpSprite = Sprite::create(imageName.asString().c_str(), Rect(0, 35, 40, 25));
  this->addChild(tmpSprite, LayerPlayer - 2);
  tmpSprite->setGlobalZOrder(LayerPlayer - 2);
  tmpSprite->setPosition(Point(X, Y + (WALLHEIGHT - FLOORHEIGHT) - 30));
  vecWall.pushBack(tmpSprite);

  
}

void Hall::createMap() {
  srand(time(nullptr));

  const float X = upLeftX - (dir % 2 == 0 ? 0 : FLOORWIDTH);
  const float Y = upLeftY + (dir % 2 == 0 ? FLOORHEIGHT : 0);
  //(X, Y) is upLeft Position;

  float curX = X, curY = Y;
  for (INT32 H = sizeHeight - 1; H >= 0; H--) {  // for height and width
    for (INT32 W = 0; W <= sizeWidth - 1; W++) {
      if ((dir % 2 == 0) && (H == 0 || H == sizeHeight - 1)) {
        if (H == 0)
          generateWall(curX, curY, LayerPlayer + 1);
        else
          generateWall(curX, curY, LayerPlayer - 1);
      } else if ((dir % 2 == 1) && (W == 0 || W == sizeWidth - 1)) {
        generateWall(curX, curY, LayerPlayer + 1);
      } else {
        generateFloor(curX, curY, LayerPlayer - 2);
      }
      // randomly generate floor and Wall

      curX += FLOORWIDTH;
    }
    curX = X, curY -= FLOORHEIGHT;
  }
}

void Hall::changePositionBy(float deltaX, float deltaY) {
  upLeftX += deltaX, upLeftY += deltaY;
  downRightX += deltaX, downRightY += deltaY;

  for (auto child : getChildren()) {
    float curX = child->getPositionX(), curY = child->getPositionY();
    child->setPosition(Point(curX + deltaX, curY + deltaY));
  }
}

bool Hall::checkPlayerPosition(Knight* knight, float& ispeedX, float& ispeedY) {
  float knightX = knight->getPositionX();
  float knightY = knight->getPositionY();

  if (dir % 2 == 1) {
    if (knightX >= upLeftX && knightX <= downRightX &&
        knightY <= upLeftY + FLOORHEIGHT + FLOORHEIGHT / 2 &&
        knightY >= downRightY - FLOORHEIGHT - FLOORHEIGHT / 4) {
      if (ispeedX > 0 && knightX >= downRightX)
        ispeedX = .0f;
      else if (ispeedX < 0 && knightX <= upLeftX)
        ispeedX = .0f;
    }
  } else {
    if (knightX >= upLeftX - FLOORWIDTH - FLOORWIDTH / 4 &&
        knightX <= downRightX + FLOORWIDTH + FLOORWIDTH / 4 &&
        knightY <= upLeftY + FLOORHEIGHT / 2 && knightY >= downRightY) {
      if (ispeedY > 0 && knightY >= upLeftY + FLOORHEIGHT / 2)
        ispeedY = .0f;
      else if (ispeedY < 0 && knightY <= downRightY)
        ispeedY = .0f;
    }
  }

  if (knightX > upLeftX - FLOORWIDTH && knightX < downRightX + FLOORWIDTH &&
      knightY < upLeftY + FLOORHEIGHT && knightY > downRightY - FLOORHEIGHT)
    return true;
  return false;
}

Vector<Bullet*>& Hall::getVecPlayerBullet() { return vecPlayerBullet; }
