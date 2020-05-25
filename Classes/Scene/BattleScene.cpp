#include "BattleScene.h"
#include "Enemy\EnemyController.h"
#include <vector>

using std::vector;

Scene* BattleScene::createScene() { return BattleScene::create(); }

void BattleScene::getNextRoom(INT32 x, INT32 y, BattleRoom* curRoom, queue<BattleRoom*>& q) {
  log("%d %d", curRoom->x, curRoom->y);
    
  if (cntRoom >= MAXROOM) {
    log("ok, rooms are built");
    return;
  }

  srand(time(nullptr));

  vector<INT32> vecDir;

  for (INT32 dir = 0; dir < CNTDIR; dir++) {
    if (curRoom->visDir[dir]) continue;

    INT32 toX = x + DIRX[dir], toY = y + DIRY[dir];
    if (toX < 0 || toX >= SIZEMTX || toY < 0 || toY >= SIZEMTX) {
      curRoom->visDir[dir] = true;
      continue;
    }

    vecDir.push_back(dir); //directions can go
  }

  log("vecSize: %d", vecDir.size());

  INT32 dir = vecDir.at(rand() % vecDir.size());
  INT32 toX = x + DIRX[dir], toY = y + DIRY[dir];

  BattleRoom* toRoom = battleRoom[toX][toY];

  if (toRoom != nullptr && toRoom != beginRoom) {
    // room was built, no not need to build again
    // just connect them if toRoom is not beginRoom
    curRoom->visDir[dir] = true;
    toRoom->visDir[(dir + 2) % CNTDIR] = true;
    return;  
  }
  
  toRoom = BattleRoom::create();

  toRoom->x = toX;
  toRoom->y = toY;

  toRoom->setCenter(curRoom->centerX + DIRX[dir] * CENTERDIS,
                                    curRoom->centerY + DIRY[dir] * CENTERDIS);

  curRoom->visDir[dir] = true;
  toRoom->visDir[(dir + 2) % CNTDIR] = true;

  toRoom->createMap();
  this->addChild(toRoom, 0);
  q.push(toRoom);
  cntRoom++;
  assert(toRoom != nullptr);
}

void BattleScene::update(float delta) {
  float iSpeed = 3.0f;
  log("inupdateFunc");

  for (INT32 y = 0; y < SIZEMTX; y++) {
    for (INT32 x = 0; x < SIZEMTX; x++) {
      if (battleRoom[x][y] == nullptr) continue;

      float curX = battleRoom[x][y]->getPositionX();
      float curY = battleRoom[x][y]->getPositionY();
      battleRoom[x][y]->setPositionX(curX - iSpeed);
      //battleRoom[x][y]->setPositionY(curY - iSpeed);
    }
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
  //add knight to scene

  initRoom();

  //this->scheduleUpdate(); //update, 60fps

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
  //25 rooms

  srand(time(nullptr));
  randomGenerate(rand() % SIZEMTX, rand() % SIZEMTX);
  //assert(battleRoom[stX][stY] != nullptr);
}

void BattleScene::randomGenerate(INT32 stX, INT32 stY) {
  Size visibleSize = Director::getInstance()->getVisibleSize();

  queue<BattleRoom*> q;

  BattleRoom* curRoom = battleRoom[stX][stY];
  curRoom = BattleRoom::create();

  curRoom->x = stX;
  curRoom->y = stY;

  curRoom->setCenter(visibleSize.width / 2,
                                  visibleSize.height / 2);
  curRoom->createMap();
  this->addChild(curRoom, 0);
  this->beginRoom = curRoom;

  q.push(curRoom);
  cntRoom++;

  while (!q.empty()) {
    BattleRoom* curRoom = q.front();
    q.pop();
    assert(curRoom != nullptr);
    // getNextRoomDirection
    getNextRoom(curRoom->x, curRoom->y, curRoom, q); 
  }
  log("%d", cntRoom);
}
