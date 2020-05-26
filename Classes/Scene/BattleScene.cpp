#include "BattleScene.h"

#include <vector>

using std::vector;

Scene* BattleScene::createScene() { return BattleScene::create(); }

void BattleScene::update(float delta) {
  float iSpeed = 3.0f;
  log("inupdateFunc");

  for (INT32 y = 0; y < SIZEMTX; y++) {
    for (INT32 x = 0; x < SIZEMTX; x++) {
      if (battleRoom[x][y] == nullptr) continue;

      float curX = battleRoom[x][y]->getPositionX();
      float curY = battleRoom[x][y]->getPositionY();
      battleRoom[x][y]->setPositionX(curX - iSpeed);
      battleRoom[x][y]->setPositionY(curY - iSpeed);
    }
  }

  for (auto hall : vecHall) {
    float curX = hall->getPositionX();
    float curY = hall->getPositionY();
    hall->setPositionX(curX - iSpeed);
    hall->setPositionY(curY - iSpeed);
  }
}

bool BattleScene::init() {
  if (!Scene::init()) {
    return false;
  }

  cntRoom = 0;

  Size visibleSize = Director::getInstance()->getVisibleSize();

  // add knight to scene
  this->knight = Knight::create();
  this->knight->bindSprite(Sprite::create("Character//Knight.png"));

  this->knight->setPosition(
      Point(visibleSize.width / 2, visibleSize.height / 2));

  this->knight->setScaleX(0.3f);
  this->knight->setScaleY(0.3f);

  this->addChild(this->knight, 4);
  // add knight to scene

  initRoom();

  this->scheduleUpdate(); //update, 60fps

  return true;
}

void BattleScene::initRoom() {
  Size visibleSize = Director::getInstance()->getVisibleSize();

  // 25 rooms
  for (INT32 y = 0; y < SIZEMTX; y++) {
    for (INT32 x = 0; x < SIZEMTX; x++) {
      battleRoom[x][y] = nullptr;
    }
  }
  // 25 rooms

  srand(time(nullptr));
  INT32 stX = rand() % SIZEMTX, stY = rand() % SIZEMTX;
  randomGenerate(stX, stY);
  
  for (INT32 y = 0; y < SIZEMTX; y++) {
    for (INT32 x = 0; x < SIZEMTX; x++) {
      if (battleRoom[x][y] == nullptr) continue;
      battleRoom[x][y]->createMap();
    }
  }

  connectRoom(battleRoom[stX][stY]);
}

void BattleScene::randomGenerate(INT32 stX, INT32 stY) {
  Size visibleSize = Director::getInstance()->getVisibleSize();

  queue<BattleRoom*> q;

  BattleRoom*& room = battleRoom[stX][stY]; // the pointer will be changed
  room = BattleRoom::create();
  this->beginRoom = room;

  room->x = stX, room->y = stY;
  log("curRoom %d %d", room->x, room->y);
  log("beginRoom %d %d", beginRoom->x, beginRoom->y);
  room->setCenter(visibleSize.width / 2, visibleSize.height / 2);
  this->addChild(room, 2);
  
  q.push(room);
  cntRoom++;

  while (!q.empty()) {
    BattleRoom* curRoom = q.front();
    q.pop();
    // getNextRoomDirection
    getToRoom(curRoom->x, curRoom->y, curRoom, q);
  }
}

void BattleScene::getToRoom(INT32 x, INT32 y, BattleRoom* curRoom,
                              queue<BattleRoom*>& q) {
  if (cntRoom >= MAXROOM) {
    log("ok, rooms are built");
    return;
  }

  srand(time(nullptr));

  vector<INT32> vecDir;

  for (INT32 dir = 0; dir < CNTDIR; dir++) {
    if (curRoom->visDir[dir]) continue;

    INT32 toX = x + DIRX[dir], toY = y + DIRY[dir];
    if (toX < 0 || toX >= SIZEMTX || toY < 0 || toY >= SIZEMTX) continue;

    vecDir.push_back(dir);  // directions can go
  }

  if (vecDir.empty()) return;

  if (curRoom != beginRoom) q.push(curRoom);  // beginRoom just connect one room

  // randomly choose direction
  INT32 dir = vecDir.at(rand() % vecDir.size());  
  INT32 toX = x + DIRX[dir], toY = y + DIRY[dir];

  if (battleRoom[toX][toY] == beginRoom) return;

  BattleRoom*& toRoom = battleRoom[toX][toY]; // the pointer will be changed

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

  this->addChild(toRoom, 2);
  q.push(toRoom);
  cntRoom++;

  assert(battleRoom[toX][toY] != nullptr);
  assert(battleRoom[toX][toY] != beginRoom);
}

void BattleScene::setHallWithWidth(Hall* hall, BattleRoom* fromRoom,
                                   BattleRoom* toRoom) {
  hall->sizeWidth =
      SIZECENTERDIS - fromRoom->sizeWidth / 2 - toRoom->sizeWidth / 2;

  hall->upLeftX =
      fromRoom->centerX + FLOORWIDTH * (fromRoom->sizeWidth / 2 + 1);
  hall->upLeftY = fromRoom->centerY + FLOORHEIGHT * (SIZEHALL / 2 - 1);

  hall->downRightX = toRoom->centerX - FLOORWIDTH * (toRoom->sizeWidth / 2 + 1);
  hall->downRightY = toRoom->centerY - FLOORHEIGHT * (SIZEHALL / 2 - 1);
  hall->createMap();
}

void BattleScene::setHallWithHeight(Hall* hall, BattleRoom* fromRoom,
                                    BattleRoom* toRoom) {
  hall->sizeHeight =
      SIZECENTERDIS - fromRoom->sizeHeight / 2 - toRoom->sizeHeight / 2;

  hall->upLeftX = fromRoom->centerX - FLOORWIDTH * (SIZEHALL / 2 - 1);
  hall->upLeftY =
      fromRoom->centerY - FLOORHEIGHT * (fromRoom->sizeHeight / 2 + 1);

  hall->downRightX = toRoom->centerX + FLOORWIDTH * (SIZEHALL / 2 - 1);
  hall->downRightY =
      toRoom->centerY + FLOORHEIGHT * (toRoom->sizeHeight / 2 + 1);
  hall->createMap();
}

void BattleScene::connectRoom(BattleRoom* curRoom) {
  assert(curRoom != nullptr);

  for (INT32 dir = 0; dir < CNTDIR; dir++) {
    if (curRoom->visDir[dir] == false) continue;
    INT32 toX = curRoom->x + DIRX[dir];
    INT32 toY = curRoom->y + DIRY[dir];

    BattleRoom* toRoom = battleRoom[toX][toY];

    Hall* hall = Hall::create();
    hall->dir = dir;

    switch (dir) {
      case RIGHT:
        setHallWithWidth(hall, curRoom, toRoom);
        break;
      case UP:
        setHallWithHeight(hall, toRoom, curRoom);
        break;
      case LEFT:
        setHallWithWidth(hall, toRoom, curRoom);
        break;
      case DOWN:
        setHallWithHeight(hall, curRoom, toRoom);
        break;
    }

    this->addChild(hall);
    vecHall.pushBack(hall);

    curRoom->visDir[dir] = false;
    toRoom->visDir[(dir + 2) % CNTDIR] = false;

    connectRoom(toRoom);
    //back trace
    curRoom->visDir[dir] = true;
    toRoom->visDir[(dir + 2) % CNTDIR] = true;
  }
}