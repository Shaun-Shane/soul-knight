#include "BattleScene.h"
#include"SetScene.h"
#include"StartScene.h"

#include <vector>
#include <cmath>

using std::vector;
using std::max;

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

  /*创建状态信息进度条*/
  auto StatusBackGround = Sprite::create("Character//StatusBackground.png");

  BloodLoadingBar = ui::LoadingBar::create("Character//StatusBlood.png");
  ArmorLoadingBar = ui::LoadingBar::create("Character//StatusArmor.png");
  MPLoadingBar = ui::LoadingBar::create("Character//StatusMP.png");

  BloodLoadingBar->setDirection(ui::LoadingBar::Direction::LEFT);
  ArmorLoadingBar->setDirection(ui::LoadingBar::Direction::LEFT);
  MPLoadingBar->setDirection(ui::LoadingBar::Direction::LEFT);

  StatusBackGround->setPosition(80, 680);
  BloodLoadingBar->setPosition(Vec2(89, 698));
  ArmorLoadingBar->setPosition(Vec2(89, 680));
  MPLoadingBar->setPosition(Vec2(89, 664));

  this->addChild(StatusBackGround, TOP);
  this->addChild(BloodLoadingBar, TOP);
  this->addChild(ArmorLoadingBar, TOP);
  this->addChild(MPLoadingBar, TOP);

  /*置顶*/
  StatusBackGround->setGlobalZOrder(TOP);
  BloodLoadingBar->setGlobalZOrder(TOP);
  ArmorLoadingBar->setGlobalZOrder(TOP);
  MPLoadingBar->setGlobalZOrder(TOP);

  /*状态数字信息*/
  HPLabel = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 15);
  armorLabel = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 15);
  MPLabel = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 15);

  HPLabel->setPosition(Vec2(89, 698));
  armorLabel->setPosition(Vec2(89, 680));
  MPLabel->setPosition(Vec2(89, 664));

  this->addChild(HPLabel, TOP);
  this->addChild(armorLabel, TOP);
  this->addChild(MPLabel, TOP);

  /*置顶*/
  HPLabel->setGlobalZOrder(TOP);
  armorLabel->setGlobalZOrder(TOP);
  MPLabel->setGlobalZOrder(TOP);

  // add knight to scene
  this->knight = Knight::create();
  this->knight->bindSprite(Sprite::create("Character//Knight1.png"), LayerPlayer);

  this->knight->setPosition(
      Point(visibleSize.width / 2, visibleSize.height / 2));

  this->addChild(this->knight);
  // add knight to scene

  miniMap = MiniMap::create(); //添加小地图
  this->addChild(miniMap);

  initRoom(); //战斗房间初始化
  initMiniMap();
  connectRoom(beginRoom); //从起始房间开始联通房间

  this->scheduleUpdate(); //60帧跟新

  return true;
}

void BattleScene::update(float delta) {
  updatePlayerPos();

  /*进度条更新*/
  BloodLoadingBar->setPercent(this->knight->getHP() * 100 / 5 );
  ArmorLoadingBar->setPercent(this->knight->getArmor() * 100 / 5 );
  MPLoadingBar->setPercent(float(this->knight->getMP()) / 200.0f * 100);

  /*状态信息更新*/
  HPLabel->setString(Value(this->knight->HP).asString() + "/5");
  armorLabel->setString(Value(this->knight->armor).asString() + "/5");
  MPLabel->setString(Value(this->knight->MP).asString() + "/200");

  if (knight->atBattleRoom == nullptr) return;

  for (auto enemy : knight->atBattleRoom->getVecEnemy()) {  //敌人AI
    if (enemy->getParent() == nullptr) continue; //死亡的敌人指针可能还未被释放
    enemy->aiOfEnemy(knight, knight->atBattleRoom);
  }
}

void BattleScene::updatePlayerPos() {
  float ispeedX = knight->moveSpeedX;
  float ispeedY = knight->moveSpeedY;

  if (abs(ispeedX) > 0 && abs(ispeedY) > 0)  //确保任意方向速度相同
    ispeedX /= sqrt(2.0f), ispeedY /= sqrt(2.0f);  

  for (INT32 y = 0; y < SIZEMTX; y++) {
    for (INT32 x = 0; x < SIZEMTX; x++) {
      if (battleRoom[x][y] == nullptr) continue;
      BattleRoom* curRoom = battleRoom[x][y];
      MiniRoom* curMiniRoom = miniMap->miniRoom[x][y];

      bool inRoom = curRoom->checkPlayerPosition(knight, ispeedX, ispeedY);
      if (inRoom) { 
        knight->bindBattleRoom(curRoom), knight->bindHall(nullptr); 

        curMiniRoom->setVisible(true);  //修改小地图状态
        if (curRoom != beginRoom && curRoom != endRoom)
          curMiniRoom->setColorWhite();
        curRoom->playerVisited = true;

        for (INT32 dir = 0; dir < CNTDIR; dir++) {
          if (curRoom->visDir[dir] == false) continue;
          BattleRoom* toRoom = battleRoom[x + DIRX[dir]][y + DIRY[dir]];
          MiniRoom* toMiniRoom =
              miniMap->miniRoom[x + DIRX[dir]][y + DIRY[dir]];
          if (toRoom->playerVisited == false) toMiniRoom->setVisible(true);

          if (curMiniRoom->miniHall[dir]->isVisible() == false)
            curMiniRoom->miniHall[dir]->setVisible(true);
        }
      } else if (curRoom->playerVisited && curRoom != beginRoom &&
                 curRoom != endRoom)
        curMiniRoom->setColorGrey();
    }
  }
  for (auto hall : vecHall) {
    bool inHall = hall->checkPlayerPosition(knight, ispeedX, ispeedY);
    if (inHall) knight->bindBattleRoom(nullptr), knight->bindHall(hall); 
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
  
  for (INT32 y = 0; y < SIZEMTX; y++) { // 25 rooms
    for (INT32 x = 0; x < SIZEMTX; x++) {
      battleRoom[x][y] = nullptr;
    }
  } // 25 roomss

  srand(time(nullptr));

  //在5 * 5的矩阵中随机选取MAXROOM个房间
  randomGenerate(SIZEMTX / 2, 1 + rand() % 3);

  for (INT32 y = 0; y < SIZEMTX; y++) { //判断房间类型并生成地图
    for (INT32 x = 0; x < SIZEMTX; x++) {
      BattleRoom* curRoom = battleRoom[x][y];
      if (curRoom == nullptr) continue;

      if (curRoom == beginRoom)
        curRoom->roomType = BEGIN;
      else if (curRoom == endRoom)
        curRoom->roomType = END;
      else curRoom->roomType = NORMAL;

      curRoom->knight = knight;
      curRoom->createMap();
    }
  }
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

  INT32 cntDirChosen = max(2U, (rand() % (vecDir.size() + 1)));

  // randomly choose direction
  for (INT32 i = 0; i < cntDirChosen; i++) {
    INT32 dirIndex = rand() % vecDir.size();
    INT32 dir = vecDir.at(dirIndex);
    vecDir.erase(vecDir.begin() + dirIndex);
    INT32 toX = x + DIRX[dir], toY = y + DIRY[dir];

    if (battleRoom[toX][toY] == beginRoom) return;

    BattleRoom*& toRoom = battleRoom[toX][toY];  // the pointer will be changed
    if (BattleRoom::createRoom(toRoom, curRoom, dir, toX, toY)) {
      this->addChild(toRoom);
      q.push(toRoom);
      endRoom = toRoom;
      cntRoom++;
    }

    assert(battleRoom[toX][toY] != nullptr);
    assert(battleRoom[toX][toY] != beginRoom);

    if (cntRoom >= MAXROOM || curRoom == beginRoom || vecDir.empty()) return;
  }
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
        color = Color4F(0.0f, 0.0f, 0.0f, 0.75f);

      miniMap->miniRoom[x][y] = MiniRoom::create();
      miniMap->miniRoom[x][y]->createRoom(x, y, color);
      miniMap->addChild(miniMap->miniRoom[x][y]);
    }
  }
}

// 确定走廊的宽度 顶点
void BattleScene::setHallWithWidth(Hall* hall, DrawNode* miniHall,
                                   const BattleRoom* fromRoom,
                                   const BattleRoom* toRoom) {
  hall->sizeWidth =
      SIZECENTERDIS - fromRoom->sizeWidth / 2 - toRoom->sizeWidth / 2 - 1;

  hall->upLeftX =
      fromRoom->centerX + FLOORWIDTH * (fromRoom->sizeWidth / 2 + 1);
  hall->upLeftY = fromRoom->centerY + FLOORHEIGHT * (SIZEHALL / 2 - 1);

  hall->downRightX = toRoom->centerX - FLOORWIDTH * (toRoom->sizeWidth / 2 + 1);
  hall->downRightY = toRoom->centerY - FLOORHEIGHT * (SIZEHALL / 2 - 1);
  hall->createMap();
  //确定小地图绘制顶点
  float downLeftX = miniMap->miniRoom[fromRoom->x][fromRoom->y]->upRightX;
  float downLeftY = miniMap->miniRoom[fromRoom->x][fromRoom->y]->downLeftY + 7;
  float upRightX = miniMap->miniRoom[toRoom->x][toRoom->y]->downLeftX;
  float upRightY = miniMap->miniRoom[toRoom->x][toRoom->y]->upRightY - 7;
  miniHall->drawSolidRect(Vec2(downLeftX, downLeftY), Vec2(upRightX, upRightY),
                          Color4F(0.9f, 0.9f, 0.9f, 0.3f));
}

// 确定走廊的高度 顶点
void BattleScene::setHallWithHeight(Hall* hall, DrawNode* miniHall,
                                    const BattleRoom* fromRoom,
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
  //确定小地图绘制顶点
  float downLeftX = miniMap->miniRoom[toRoom->x][toRoom->y]->downLeftX + 7;
  float downLeftY = miniMap->miniRoom[toRoom->x][toRoom->y]->upRightY;
  float upRightX = miniMap->miniRoom[fromRoom->x][fromRoom->y]->upRightX - 7;
  float upRightY = miniMap->miniRoom[fromRoom->x][fromRoom->y]->downLeftY;
  miniHall->drawSolidRect(Vec2(downLeftX, downLeftY), Vec2(upRightX, upRightY),
                          Color4F(0.9f, 0.9f, 0.9f, 0.3f));
}

//dfs联通房间
void BattleScene::connectRoom(BattleRoom* curRoom) {
  assert(curRoom != nullptr);

  for (INT32 dir = 0; dir < CNTDIR; dir++) { //4 directions
    if (curRoom->visDir[dir] == false) continue;
    INT32 toX = curRoom->x + DIRX[dir];
    INT32 toY = curRoom->y + DIRY[dir];

    auto toRoom = battleRoom[toX][toY];
    auto curMiniRoom = miniMap->miniRoom[curRoom->x][curRoom->y];
    auto toMiniRoom = miniMap->miniRoom[toX][toY];
    
    auto miniHall = DrawNode::create(); //生成小地图走廊
    miniHall->setGlobalZOrder(TOP);
    miniHall->setVisible(false);

    auto hall = Hall::create(); //生成地图走廊
    hall->knight = knight;
    hall->dir = dir;

    switch (dir) {
      case RIGHT:
        setHallWithWidth(hall, miniHall, curRoom, toRoom);
        break;
      case UP:
        setHallWithHeight(hall, miniHall, toRoom, curRoom);
        break;
      case LEFT:
        setHallWithWidth(hall, miniHall, toRoom, curRoom);
        break;
      case DOWN:
        setHallWithHeight(hall, miniHall, curRoom, toRoom);
        break;
    }

    this->addChild(hall);
    miniMap->addChild(miniHall);
    vecHall.pushBack(hall);
    
    curRoom->visDir[dir] = false; //标记不能来回连接
    curMiniRoom->miniHall[dir] = miniHall;
    toRoom->visDir[(dir + 2) % CNTDIR] = false; //标记不能来回连接
    toMiniRoom->miniHall[(dir + 2) % CNTDIR] = miniHall;

    connectRoom(toRoom);
    
    curRoom->visDir[dir] = true; //back trace
    toRoom->visDir[(dir + 2) % CNTDIR] = true;
  }
}

/*退出游戏*/
void BattleScene::menuCloseCallbackEnd(Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, StartScene::createScene()));
}

/*进入设置面板*/
void BattleScene::menuCloseCallbackSet(Ref* pSender)
{
	CCScheduler* defaultScheduler = CCDirector::sharedDirector()->getScheduler();
	defaultScheduler->pauseTarget(this);
	Director::getInstance()->pushScene(TransitionFade::create(1.0f, SetScene::createScene()));
	defaultScheduler->resumeTarget(this);
}