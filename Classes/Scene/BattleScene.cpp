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
      battleRoom[x][y]->changePositionBy(0, -iSpeed);

      BattleRoom* curRoom = battleRoom[x][y];
      {
        float xx = knight->getPositionX(), yy = knight->getPositionY();
        if ( xx >= curRoom->upLeftX && xx <= curRoom->downRightX && yy <= curRoom->upLeftY && yy >= curRoom->downRightY) {
          curRoom->closeDoor();
        } else
          curRoom->openDoor();
      }

    }
  }

  for (auto hall : vecHall) {
    hall->changePositionBy(0, -iSpeed);
  }
}

bool BattleScene::init() {
  if (!Scene::init()) {
    return false;
  }

  Size visibleSize = Director::getInstance()->getVisibleSize();

  // add knight to scene
  this->knight = Knight::create();
  this->knight->bindSprite(Sprite::create("Character//Knight.png"), LayerPlayer);

  this->knight->setPosition(
      Point(visibleSize.width / 2, visibleSize.height / 2));

  this->knight->setScaleX(0.3f);
  this->knight->setScaleY(0.3f);


  this->addChild(this->knight);
  // add knight to scene

  initRoom(); //战斗房间初始化

  this->scheduleUpdate(); //60帧跟新

  return true;
}

void BattleScene::initRoom() {
  Size visibleSize = Director::getInstance()->getVisibleSize();

  cntRoom = 0;
  
  // 25 rooms
  for (INT32 y = 0; y < SIZEMTX; y++) {
    for (INT32 x = 0; x < SIZEMTX; x++) {
      battleRoom[x][y] = nullptr;
    }
  }
  // 25 rooms

  srand(time(nullptr));
  INT32 stX = rand() % SIZEMTX, stY = rand() % SIZEMTX;

  //在5 * 5的矩阵中随机选取MAXROOM个房间
  randomGenerate(stX, stY);
  
  for (INT32 y = 0; y < SIZEMTX; y++) {
    for (INT32 x = 0; x < SIZEMTX; x++) {
      BattleRoom* curRoom = battleRoom[x][y];
      if (curRoom == nullptr) continue;

      if (curRoom == endRoom) { // add portal to endRoom
        curRoom->sizeWidth -= 8, curRoom->sizeHeight -= 8;
        curRoom->createMap();

        Sprite* portal = Sprite::create("Map//portal3.png");
        portal->setPosition(Point(curRoom->centerX, curRoom->centerY));
        curRoom->addChild(portal);
        portal->setGlobalZOrder(4);

        curRoom->portal = portal;
      } else {
        curRoom->createMap();
      } 
      
    }
  }
  //从起始房间开始联通房间
  connectRoom(battleRoom[stX][stY]);
}

void BattleScene::randomGenerate(INT32 stX, INT32 stY) {
  //从一个房间开始向四周扩展 类似bfs
  Size visibleSize = Director::getInstance()->getVisibleSize();

  queue<BattleRoom*> q;

  BattleRoom*& room = battleRoom[stX][stY]; // the pointer will be changed
  room = BattleRoom::create();
  this->beginRoom = room;

  room->x = stX, room->y = stY;
  log("curRoom %d %d", room->x, room->y);
  log("beginRoom %d %d", beginRoom->x, beginRoom->y);
  room->setCenter(visibleSize.width / 2, visibleSize.height / 2);
  this->addChild(room, 0);
  
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
  this->endRoom = toRoom; //set endRoom

  toRoom->x = toX, toRoom->y = toY;
  log("%d %d", toX, toY);
  toRoom->setCenter(curRoom->centerX + DIRX[dir] * CENTERDIS,
                    curRoom->centerY + DIRY[dir] * CENTERDIS);

  curRoom->visDir[dir] = true;
  toRoom->visDir[(dir + 2) % CNTDIR] = true;

  this->addChild(toRoom, 0);
  q.push(toRoom);
  cntRoom++;

  assert(battleRoom[toX][toY] != nullptr);
  assert(battleRoom[toX][toY] != beginRoom);
}
// 确定走廊的宽度 顶点
void BattleScene::setHallWithWidth(Hall* hall, const BattleRoom* fromRoom,
                                   const BattleRoom* toRoom) {
  hall->sizeWidth =
      SIZECENTERDIS - fromRoom->sizeWidth / 2 - toRoom->sizeWidth / 2 - 1;

  hall->upLeftX =
      fromRoom->centerX + FLOORWIDTH * (fromRoom->sizeWidth / 2 + 1);
  hall->upLeftY = fromRoom->centerY + FLOORHEIGHT * (SIZEHALL / 2 - 1);

  hall->downRightX = toRoom->centerX - FLOORWIDTH * (toRoom->sizeWidth / 2 + 1);
  hall->downRightY = toRoom->centerY - FLOORHEIGHT * (SIZEHALL / 2 - 1);
  hall->createMap();
}
// 确定走廊的高度 顶点
void BattleScene::setHallWithHeight(Hall* hall, const BattleRoom* fromRoom,
                                    const BattleRoom* toRoom) {
  hall->sizeHeight =
      SIZECENTERDIS - fromRoom->sizeHeight / 2 - toRoom->sizeHeight / 2 - 1;

  hall->upLeftX = fromRoom->centerX - FLOORWIDTH * (SIZEHALL / 2 - 1);
  hall->upLeftY =
      fromRoom->centerY - FLOORHEIGHT * (fromRoom->sizeHeight / 2 + 1);

  hall->downRightX = toRoom->centerX + FLOORWIDTH * (SIZEHALL / 2 - 1);
  hall->downRightY =
      toRoom->centerY + FLOORHEIGHT * (toRoom->sizeHeight / 2 + 1);
  hall->createMap();
}
//dfs联通房间
void BattleScene::connectRoom(BattleRoom* curRoom) {
  assert(curRoom != nullptr);

  for (INT32 dir = 0; dir < CNTDIR; dir++) { //4 directions
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

    this->addChild(hall, 0);
    vecHall.pushBack(hall);
    //标记不能来回访问
    curRoom->visDir[dir] = false;
    toRoom->visDir[(dir + 2) % CNTDIR] = false;

    connectRoom(toRoom);
    //back trace
    curRoom->visDir[dir] = true;
    toRoom->visDir[(dir + 2) % CNTDIR] = true;
  }
}