#include "BattleScene.h"
#include"SetScene.h"

#include <vector>

using std::vector;

Scene* BattleScene::createScene() { return BattleScene::create(); }

bool BattleScene::init() {
  if (!Scene::init()) {
    return false;
  }

  Size visibleSize = Director::getInstance()->getVisibleSize();

  /*创建关闭按钮以及设置按钮*/
  auto setImg = MenuItemImage::create(
	  "set.png",
	  "set.png",
	  CC_CALLBACK_1(BattleScene::menuCloseCallbackSet, this));
  auto exitImg = MenuItemImage::create(
	  "exit.png",
	  "exit01.png",
	  CC_CALLBACK_1(BattleScene::menuCloseCallbackEnd, this));

  auto Menu01 = Menu::create(setImg, NULL);
  auto Menu02 = Menu::create(exitImg, NULL);

  Menu01->setPosition(1060, 660);
  Menu02->setPosition(1200, 660);

  this->addChild(Menu01);
  this->addChild(Menu02);

  /*添加两个精灵使两个菜单的图片得到显示*/
  auto setSprite = Sprite::create("set.png");
  auto exitSprite = Sprite::create("exit.png");

  setSprite->setPosition(1060, 660);
  exitSprite->setPosition(1200, 660);

  this->addChild(setSprite);
  this->addChild(exitSprite);

  /*图片置顶*/
  setSprite->setGlobalZOrder(TOP);
  exitSprite->setGlobalZOrder(TOP);

  // add knight to scene
  this->knight = Knight::create();
  this->knight->bindSprite(Sprite::create("Character//Knight.png"), LayerPlayer);

  this->knight->setPosition(
      Point(visibleSize.width / 2, visibleSize.height / 2));

  this->knight->setScaleX(0.3f);
  this->knight->setScaleY(0.3f);


  this->addChild(this->knight);
  // add knight to scene

  miniMap = MiniMap::create(); //添加小地图
  this->addChild(miniMap);

  initRoom(); //战斗房间初始化
  initMiniMap();

  this->scheduleUpdate(); //60帧跟新

  return true;
}

void BattleScene::update(float delta) {
  float ispeedX = knight->moveSpeedX;
  float ispeedY = knight->moveSpeedY;

  for (INT32 y = 0; y < SIZEMTX; y++) {
    for (INT32 x = 0; x < SIZEMTX; x++) {
      if (battleRoom[x][y] == nullptr) continue;
      BattleRoom* curRoom = battleRoom[x][y];

      curRoom->checkPlayerPosition(knight, ispeedX, ispeedY);
    }
  }
  for (auto hall : vecHall) {
    hall->checkPlayerPosition(knight, ispeedX, ispeedY);
  }

  for (INT32 y = 0; y < SIZEMTX; y++) {
    for (INT32 x = 0; x < SIZEMTX; x++) {
      if (battleRoom[x][y] == nullptr) continue;
      BattleRoom* curRoom = battleRoom[x][y];
      curRoom->changePositionBy(-ispeedX, -ispeedY);
    }
  }
  for (auto hall : vecHall) {
    hall->changePositionBy(-ispeedX, -ispeedY);
  }
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

  //在5 * 5的矩阵中随机选取MAXROOM个房间
  randomGenerate(SIZEMTX / 2, 1 + rand() % 3);
  
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
  connectRoom(beginRoom);
}

void BattleScene::randomGenerate(INT32 stX, INT32 stY) {
  //从一个房间开始向四周扩展
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

//随机生成下一个房间
void BattleScene::getToRoom(INT32 x, INT32 y, BattleRoom* curRoom,
                              queue<BattleRoom*>& q) {
  if (cntRoom >= MAXROOM) return;

  srand(time(nullptr));

  vector<INT32> vecDir;

  for (INT32 dir = 0; dir < CNTDIR; dir++) {
    if (curRoom->visDir[dir]) continue;

    INT32 toX = x + DIRX[dir], toY = y + DIRY[dir];
    if (toX < 0 || toX >= SIZEMTX || toY < 0 || toY >= SIZEMTX) continue;

    vecDir.push_back(dir);  // directions can go
  }

  if (vecDir.empty()) return;

  // randomly choose direction
  INT32 dirIndex = rand() % vecDir.size();
  INT32 dir = vecDir.at(dirIndex);  
  vecDir.erase(vecDir.begin() + dirIndex);
  INT32 toX = x + DIRX[dir], toY = y + DIRY[dir];

  if (battleRoom[toX][toY] == beginRoom) return;

  BattleRoom*& toRoom1 = battleRoom[toX][toY]; // the pointer will be changed
  BattleRoom::createRoom(toRoom1, curRoom, dir, toX, toY);

  this->addChild(toRoom1, 0);
  q.push(toRoom1);
  endRoom = toRoom1;
  cntRoom++;
  
  if (cntRoom >= MAXROOM || curRoom == beginRoom || vecDir.empty()) return;

  dir = vecDir.at(rand() % vecDir.size());
  toX = x + DIRX[dir], toY = y + DIRY[dir];

  if (battleRoom[toX][toY] == beginRoom) return;

  BattleRoom*& toRoom2 = battleRoom[toX][toY];  // the pointer will be changed
  BattleRoom::createRoom(toRoom2, curRoom, dir, toX, toY);

  this->addChild(toRoom2, 0);
  cntRoom++;

  assert(battleRoom[toX][toY] != nullptr);
  assert(battleRoom[toX][toY] != beginRoom);
}

void BattleScene::initMiniMap() {
  for (int x = 0; x < SIZEMTX; x++) {
    for (int y = 0; y < SIZEMTX; y++) {
      if (battleRoom[x][y] == nullptr) continue;
      Color4F color;
      if (battleRoom[x][y] == beginRoom)
        color = Color4F(.0f, 0.9f, .0f, 0.5f);
      else if (battleRoom[x][y] == endRoom)
        color = Color4F(.0f, .0f, 1.0f, 0.7f);
      else
        color = Color4F(0.9f, 0.9f, 0.9f, 0.35f);

      miniMap->miniRoom[x][y] = MiniRoom::create();
      miniMap->miniRoom[x][y]->createRoom(x, y, color);
      miniMap->addChild(miniMap->miniRoom[x][y]);
    }
  }
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

/*退出游戏*/
void BattleScene::menuCloseCallbackEnd(Ref* pSender)
{
	Director::getInstance()->end();
}

/*进入设置面板*/
void BattleScene::menuCloseCallbackSet(Ref* pSender)
{
	CCScheduler* defaultScheduler = CCDirector::sharedDirector()->getScheduler();
	defaultScheduler->pauseTarget(this);
	Director::getInstance()->pushScene(TransitionFade::create(1.0f, SetScene::createScene()));
	defaultScheduler->resumeTarget(this);
}