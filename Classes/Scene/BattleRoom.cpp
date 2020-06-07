#include "BattleRoom.h"

bool BattleRoom::init() {
  centerX = .0f, centerY = .0f;
  upLeftX = .0f, upLeftY = .0f;
  downRightX = .0f, downRightY = .0f;
  x = 0, y = 0;
  roomType = NORMAL;
  sizeHeight = SIZEROOM, sizeWidth = SIZEROOM;

  memset(visDir, false, sizeof(visDir));
  playerVisited = false;

  portal = nullptr, knight = nullptr;

  this->scheduleUpdate();
  return true;
}

void BattleRoom::update(float delta) {
  this->bulletMove();
  this->playerBulletCollistionCheck();

}

void BattleRoom::createRoom(BattleRoom*& toRoom, BattleRoom* curRoom, INT32 dir, INT32 toX, INT32 toY) {
  if (toRoom != nullptr) {
    // room was built, no not need to build again
    curRoom->visDir[dir] = true;
    toRoom->visDir[(dir + 2) % CNTDIR] = true;
    // just connect them if toRoom is not beginRoom
    return;
  }

  toRoom = BattleRoom::create();

  toRoom->x = toX, toRoom->y = toY;
  log("%d %d", toX, toY);
  toRoom->setCenter(curRoom->centerX + DIRX[dir] * CENTERDIS,
                    curRoom->centerY + DIRY[dir] * CENTERDIS);

  curRoom->visDir[dir] = true;
  toRoom->visDir[(dir + 2) % CNTDIR] = true;
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

  if (roomType == END) {
    sizeWidth -= 8, sizeHeight -= 8;
    Sprite* portal = Sprite::create("Map//portal3.png");
    portal->setPosition(Point(centerX, centerY));
    addChild(portal);
    portal->setGlobalZOrder(LayerPlayer - 1);

    this->portal = portal;
  }

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
      } // randomly generate floor and Wall
      
      curX += FLOORWIDTH;
    }
    curX = X, curY -= FLOORHEIGHT;
  }

  if (roomType == NORMAL) createEnemy(); 
}

void BattleRoom::createEnemy() {
  srand(time(nullptr));

  for (int i = 1; i <= 5; i++) {
    Enemy* enemy = Enemy::create();
    enemy->bindSprite(Sprite::create("Enemy//shooter.png"), LayerPlayer - 1);
    float enemyX = upLeftX + rand() % static_cast<INT32>(downRightX - upLeftX);
    float enemyY =
        downRightY + rand() % static_cast<INT32>(upLeftY - downRightY);
    this->addChild(enemy);
    enemy->setPosition(Point(enemyX, enemyY));
    vecEnemy.pushBack(enemy);
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

bool BattleRoom::checkPlayerPosition(Knight* knight, float& ispeedX,
                                     float& ispeedY) {
  float knightX = knight->getPositionX();
  float knightY = knight->getPositionY();

  if (knightX >= upLeftX - FLOORWIDTH && knightX <= downRightX + FLOORWIDTH &&
      knightY <= upLeftY + FLOORHEIGHT && knightY >= downRightY - FLOORHEIGHT) {
    // log("%d %d %d %d", visDir[0], visDir[1], visDir[2], visDir[3]);
    if (vecEnemy.empty()) openDoor();  //怪物全部击杀开门
    else closeDoor();

    if (!vecEnemy.empty()) {
      if (ispeedX > 0 && knightX >= downRightX)
        ispeedX = .0f;
      if (ispeedX < 0 && knightX <= upLeftX)
        ispeedX = .0f;
      if (ispeedY > 0 && knightY >= upLeftY + 20)
        ispeedY = .0f;
      if (ispeedY < 0 && knightY <= downRightY)
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
        if (ispeedX < 0 && knightX <= upLeftX)
          ispeedX = .0f;
        if (ispeedY > 0 && knightY >= upLeftY + 20)
          ispeedY = .0f;
        if (ispeedY < 0 && knightY <= downRightY)
          ispeedY = .0f;
      }
    }

    return true;
  }
  return false;
}

Vector<Enemy*>& BattleRoom::getVecEnemy() { return vecEnemy; }


Vector<Sprite*>& BattleRoom::getVecEnemyBullet() { return vecEnemyBullet; }




void BattleRoom::playerBulletCollistionCheck()
{
  
  for (int i = 0; i < vecPlayerBullet.size(); ++i)
  {
    auto bullet = vecPlayerBullet.at(i);
    Rect bulletRect = bullet->getBoundingBox();
    for (int j = 0; j < vecEnemy.size(); ++j)
    {
      auto enemy = vecEnemy.at(j);
      if (enemy->getParent() == nullptr) continue;
      Rect enemyRect = enemy->getBoundingBox();
      if (bulletRect.intersectsRect(enemyRect))
      {
        INT32 hp = knight->getHP();
        enemy->deductHP(bullet->getAttack());
        if ((enemy->getHP()) <= 0) {
          enemy->removeFromParent();
          if (this->allKilled() == true) vecEnemy.clear();
        }
        bullet->removeFromParent();
        vecPlayerBullet.eraseObject(bullet);
        --i;
        break;
      }
    }
  }
}

bool BattleRoom::allKilled()
{
  bool allKilled = true;
  for (auto e : vecEnemy) {
    if (e->getParent() != nullptr) allKilled = false;
  }
  return allKilled;
}