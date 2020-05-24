#include "BattleScene.h"
#include "Enemy\EnemyController.h"
#include <vector>

using std::vector;

Scene* BattleScene::createScene() { return BattleScene::create(); }

void BattleScene::getNextRoomDir(INT32 x, INT32 y, BattleRoom*& curRoom, queue<BattleRoom*>& q) {
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

  INT32 dir = vecDir.at(rand() % vecDir.size());
  INT32 toX = x + DIRX[dir], toY = y + DIRY[dir];

  if (battleRoom[toX][toY] != nullptr) {
    curRoom->visDir[dir] = true;
    battleRoom[toX][toY]->visDir[(dir + 2) % CNTDIR] = true;
    return;  // room was built, no not need to build again;
  }
  
  battleRoom[toX][toY] = BattleRoom::create();

  battleRoom[toX][toY]->x = toX;
  battleRoom[toX][toY]->y = toY;

  battleRoom[toX][toY]->setCenter(curRoom->centerX + DIRX[dir] * CENTERDIS,
                                    curRoom->centerY + DIRY[dir] * CENTERDIS);

  curRoom->visDir[dir] = true;
  battleRoom[toX][toY]->visDir[(dir + 2) % CNTDIR] = true;

  battleRoom[toX][toY]->createMap();
  this->addChild(battleRoom[toX][toY], 0);
  q.push(battleRoom[toX][toY]);
  cntRoom++;
  assert(battleRoom[toX][toY] != nullptr);
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

  //add enemyContoller into scene
  //EnemyController* enemyCtr = EnemyController::create();
  //this->addChild(enemyCtr, 4);
  //enemyCtr->bindKnight(knight);
  //add enemyContoller into scene

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
  //25 rooms
 
  auto floorSprite = Sprite::create("Map//floor1.png");
  auto wallSprite = Sprite::create("Map//wall1.png");

  srand(time(nullptr));
  //INT32 stX = rand() % SIZEMTX, stY = rand() % SIZEMTX;
  randomGenerate(2, 2);
  //assert(battleRoom[stX][stY] != nullptr);
}

void BattleScene::randomGenerate(INT32 stX, INT32 stY) {
  Size visibleSize = Director::getInstance()->getVisibleSize();

  queue<BattleRoom*> q;
  battleRoom[stX][stY] = BattleRoom::create();

  battleRoom[stX][stY]->x = stX;
  battleRoom[stX][stY]->y = stY;

  battleRoom[stX][stY]->setCenter(visibleSize.width / 2,
                                  visibleSize.height / 2);
  battleRoom[stX][stY]->createMap();
  this->addChild(battleRoom[stX][stY], 0);

  q.push(battleRoom[stX][stY]);
  cntRoom++;

  while (!q.empty()) {
    BattleRoom* curRoom = q.front();
    q.pop();
    assert(curRoom != nullptr);
    // getNextRoomDirection
    getNextRoomDir(curRoom->x, curRoom->y, curRoom, q); 
  }
  log("%d", cntRoom);
}
