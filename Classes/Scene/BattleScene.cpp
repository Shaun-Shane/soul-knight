#include "BattleScene.h"
#include "SetScene.h"
#include "StartScene.h"

using std::vector;
using std::max;

Scene* BattleScene::createScene() { return BattleScene::create(); }

INT32 BattleScene::getSceneNumber() { return battleSceneNumber; }

const vector<string>& BattleScene::getVecSceneType() { return vecSceneType; }

/*退出游戏*/
void BattleScene::menuCloseCallbackEnd(Ref* pSender) { Director::getInstance()->popScene(); }

bool BattleScene::init() {
	if (!Scene::init()) {
		return false;
	}
	log("%d", battleSceneNumber);
	for (auto s : vecSceneType) {
		log("%s", Value(s).asString().c_str());
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

	/*金币数,关卡数*/
	goldLabel = Label::createWithTTF("gold : 0", "fonts/Marker Felt.ttf", 30);
	gameLevelLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 30);

	goldLabel->setPosition(890, 680);
	gameLevelLabel->setPosition(1130, 400);

	this->addChild(goldLabel);
	this->addChild(gameLevelLabel);

	goldLabel->setGlobalZOrder(TOP);
	gameLevelLabel->setGlobalZOrder(TOP);

	// add knight to scene remove it from prev scene

	this->addChild(knight);

	knight->moveSpeedX = knight->moveSpeedY = .0f;

	knight->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	// add knight to scene

	miniMap = MiniMap::create(); //添加小地图
	this->addChild(miniMap);

	initRoom(); //战斗房间初始化 
	initMiniMap();
	connectRoom(beginRoom); //从起始房间开始联通房间

	if (endRoom->roomType == BOSS) {
		BOSSBloodBg = Sprite::create("Enemy//bossBloodBg.png");

		BOSSLoadingBar = ui::LoadingBar::create("Enemy//bossBlood.png");

		BOSSLoadingBar->setDirection(ui::LoadingBar::Direction::LEFT);

		BOSSBloodBg->setPosition(Vec2(600, 650));
		BOSSLoadingBar->setPosition(Vec2(600, 650));

		BOSSLoadingBar->setOpacity(150);

		this->addChild(BOSSBloodBg, TOP);
		this->addChild(BOSSLoadingBar, TOP);

		/*置顶*/
		BOSSBloodBg->setGlobalZOrder(TOP);
		BOSSLoadingBar->setGlobalZOrder(TOP);

		BOSSBloodBg->setVisible(false);
		BOSSLoadingBar->setVisible(false);//BOSS未出现时隐藏血条
	}
	this->scheduleUpdate(); //60帧跟新
	return true;
}

void BattleScene::update(float delta) {
#ifndef DEBUG
	if (knight->HP <= 0) { //人物死亡 回到安全地图
		this->cleanup();
		auto textLabel = Label::create("You are dead! Returning to SafeScene...",
			"fonts/Marker Felt.ttf", 40);
		textLabel->setPosition(Point(640, 360));
		textLabel->setGlobalZOrder(TOP);
		this->addChild(textLabel);

		auto blink = Blink::create(2.0f, 3);
		auto fadeOut = FadeOut::create(2.0f);

		auto callFunc = CallFunc::create([&]() {
			knight->removeFromParent();
			Director::getInstance()->replaceScene(
				TransitionFade::create(2.0f, SafeScene::createScene()));
			});

		knight->runAction(Sequence::create(Spawn::create(blink, fadeOut, NULL), callFunc, NULL));
	}
#endif 

	updateGold();//更新金币

	updateLevel();//更新关卡数

	knight->resumeArmor(); //更新护甲

	updatePlayerPos(); //画面位置更新
	updatePlayerInfoBar(); //进度条更新

	updateEnemy(); //更新敌人
	updateBoss();
	updateBossInfoBar(); //更新boss信息

	checkEndRoom(); //检查终点
}

void BattleScene::updatePlayerPos() {
	float ispeedX = knight->moveSpeedX;
	float ispeedY = knight->moveSpeedY;

	if (ispeedX > 0) ispeedX += knight->getMoveSpeedBuff();
	else if (ispeedX < 0) ispeedX -= knight->getMoveSpeedBuff();
	if (ispeedY > 0) ispeedY += knight->getMoveSpeedBuff();
	else if (ispeedY < 0) ispeedY -= knight->getMoveSpeedBuff();

	if (abs(ispeedX) > 0 && abs(ispeedY) > 0 && abs(abs(ispeedX) - abs(ispeedY)) < 0.0001f)
		//确保任意方向速度相同
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
				if (curRoom->roomType == NORMAL)
					curMiniRoom->setColorWhite();
				curRoom->playerVisited = true;

				for (INT32 dir = 0; dir < CNTDIR; dir++) {
					if (curRoom->visDir[dir] == false) continue;
					BattleRoom* toRoom = battleRoom[x + DIRX[dir]][y + DIRY[dir]];
					//修改迷你地图房间和走廊是否可见
					MiniRoom* toMiniRoom =
						miniMap->miniRoom[x + DIRX[dir]][y + DIRY[dir]];
					if (toRoom->playerVisited == false) toMiniRoom->setVisible(true);

					if (curMiniRoom->miniHall[dir]->isVisible() == false)
						curMiniRoom->miniHall[dir]->setVisible(true);
				}
			}
			else if (curRoom->playerVisited && curRoom->roomType == NORMAL)
				curMiniRoom->setColorGrey();
		}
	}
	for (auto hall : vecHall) {
		bool inHall = hall->checkPlayerPosition(knight, ispeedX, ispeedY);
		if (inHall) knight->bindBattleRoom(nullptr), knight->bindHall(hall);
	}

	for (INT32 y = 0; y < SIZEMTX; y++) { //修改所有子类位置
		for (INT32 x = 0; x < SIZEMTX; x++) {
			if (battleRoom[x][y] == nullptr) continue;
			BattleRoom* curRoom = battleRoom[x][y];
			curRoom->centerX -= ispeedX, curRoom->centerY -= ispeedY;
			curRoom->changePositionBy(-ispeedX, -ispeedY);
		}
	}
	for (auto hall : vecHall) { //修改所有子类位置
		hall->changePositionBy(-ispeedX, -ispeedY);
	}
}

void BattleScene::updatePlayerInfoBar() {  //更新人物信息显示
  /*进度条更新*/
	BloodLoadingBar->setPercent(knight->getHP() * 100.0f / knight->maxHP);
	ArmorLoadingBar->setPercent(knight->getArmor() * 100.0f / knight->maxArmor);
	MPLoadingBar->setPercent(knight->getMP() * 100.0f / knight->maxMP);

	/*状态信息更新*/
	HPLabel->setString(Value(knight->HP).asString() + "/" +
		Value(knight->maxHP).asString());
	armorLabel->setString(Value(knight->armor).asString() + "/" +
		Value(knight->maxArmor).asString());
	MPLabel->setString(Value(knight->MP).asString() + "/" +
		Value(knight->maxMP).asString());
}

void BattleScene::updateEnemy() { //更新敌人
	if (knight->atBattleRoom == nullptr) return;

	if (knight->atBattleRoom->allKilled()) {
		auto curRoom = knight->atBattleRoom;

		for (INT32 dir = 0; dir < CNTDIR; dir++) { //该房间怪物击杀后 相邻房间生成怪物
			if (!curRoom->visDir[dir]) continue;
			auto toRoom = battleRoom[curRoom->x + DIRX[dir]][curRoom->y + DIRY[dir]];
			if (toRoom->allKilled() == false || toRoom->playerVisited) continue;

			if (toRoom->roomType == NORMAL)
				toRoom->createEnemy();
			else if (toRoom->roomType == BOSS)
				toRoom->createBoss();
		}
		return;
	}

	auto& vecEnemy = knight->atBattleRoom->getVecEnemy();
	while (!vecEnemy.empty() && vecEnemy.back()->getParent() == nullptr &&
		vecEnemy.back()->getIsKilled()) {
		vecEnemy.popBack();
	}

	if (!vecEnemy.empty() && !vecEnemy.back()->getIsAdded()) {  //生成下一波怪物
		INT32 addChildNum =
			std::min(static_cast<INT32>(vecEnemy.size()), 4 + rand() % 3);
		for (auto it = vecEnemy.rbegin();
			it != vecEnemy.rbegin() + addChildNum && it != vecEnemy.rend(); it++) {
			float enemyX =
				knight->atBattleRoom->centerX + (rand() * 2 - RAND_MAX) % 300;
			float enemyY =
				knight->atBattleRoom->centerY + (rand() * 2 - RAND_MAX) % 300;

			(*it)->setPosition(Point(enemyX, enemyY));
			(*it)->setIsAdded(true);
			(*it)->setIsKilled(true);  //暂时设为死亡 防止立刻生成攻击玩家
			(*it)->getSprite()->setOpacity(80);
			knight->atBattleRoom->addChild(*it);  //分批次添加到场景
		}

		for (auto& enemy : vecEnemy) { //添加进场动画
			if (enemy->getParent() != nullptr && enemy->getIsKilled()) {
				auto FadeIn = FadeTo::create(0.4f, 255);
				auto callFunc = CallFunc::create([&]() {
					/*设为存活 可以行动*/
					enemy->setIsKilled(false);
					});

				enemy->getSprite()->runAction(Sequence::create(FadeIn, callFunc, NULL));
			}
		}
	}

	for (auto enemy : vecEnemy) {  //敌人AI
		if (enemy->getParent() == nullptr || enemy->getIsKilled()) continue;
		//防止死亡的敌人继续攻击
		enemy->aiOfEnemy(knight, knight->atBattleRoom);
	}
}

void BattleScene::updateBoss() {
	if (knight->atBattleRoom == endRoom && endRoom->roomType == BOSS) {
		auto boss = endRoom->getBoss();
		if (boss->getParent() == nullptr || boss->getIsKilled()) return;
		boss->aiOfBoss(knight, knight->atBattleRoom);
	}
}

void BattleScene::updateBossInfoBar() {
	if (knight->atBattleRoom == endRoom && endRoom->roomType == BOSS) {
		auto boss = endRoom->getBoss();
		if (boss->getHP() > 0) {  // boss 存活 显示血条
			BOSSBloodBg->setVisible(true);
			BOSSLoadingBar->setVisible(true);

			BOSSLoadingBar->setPercent(boss->getHP() * 100.0f / boss->getMaxHP());  //血量更新
		}
		else {  // boss 死亡 隐藏血条
			BOSSBloodBg->setVisible(false);
			BOSSLoadingBar->setVisible(false);
		}
	}
	else if (endRoom->roomType == BOSS) {
		BOSSBloodBg->setVisible(false);
		BOSSLoadingBar->setVisible(false);
	}
}

void BattleScene::updateGold() {
	goldLabel->setString("gold : " + Value(knight->gold).asString());
}

void BattleScene::updateLevel() {
	INT32 frontLevel = (battleSceneNumber + 4) / 5;
	INT32 afterLevel = battleSceneNumber - (frontLevel - 1) * 5;
	gameLevelLabel->setString(Value(frontLevel).asString() + "-" + Value(afterLevel).asString());
}

void BattleScene::checkEndRoom() { //检查房间终点
	if (knight->atBattleRoom == nullptr) return;

	if (knight->atBattleRoom == endRoom) {  //到达终点 进入
		if (endRoom->portal == nullptr) {
			if (endRoom->boss->getParent() == nullptr || endRoom->boss->getHP() <= 0) {
				//boss被击杀 加入传送门
				Sprite* portal = Sprite::create("Map//portal3.png");
				portal->setPosition(Point(endRoom->centerX, endRoom->centerY));
				endRoom->portal = portal;

				endRoom->addChild(portal);
				portal->setGlobalZOrder(LayerPlayer - 1);
			}
			return;
		}

		if (knight->goIntoPortal) {
			knight->goIntoPortal = false;
			BattleScene::knight->retain();
			BattleScene::knight->removeFromParent();  //从该场景移除
			BattleScene::battleSceneNumber++;         //关卡编号+1

			INT32 num = BattleScene::battleSceneNumber;
			num = num % 5 == 0 ? num / 5 : num / 5 + 1;
			if (num % vecSceneType.size() == 1 && num != 1) { //每过size关再随机打乱一次
				srand(static_cast<unsigned int>(time(nullptr)));
				std::random_shuffle(BattleScene::vecSceneType.begin(),
					BattleScene::vecSceneType.end());
			}

			assert(BattleScene::knight->getParent() == nullptr);

			this->cleanup();
			this->removeAllChildren();           //释放
			Director::getInstance()->replaceScene(  //进入下一个场景
				TransitionFade::create(2.0f, BattleScene::createScene()));
		}
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

	srand(static_cast<unsigned int>(time(nullptr)));

	//在5 * 5的矩阵中随机选取MAXROOM个房间
	randomGenerate(SIZEMTX / 2, 1 + rand() % 3);
	//设置房间类型
	setRoomType();

	for (INT32 y = 0; y < SIZEMTX; y++) {  // 25 rooms
		for (INT32 x = 0; x < SIZEMTX; x++) {
			BattleRoom* curRoom = battleRoom[x][y];
			if (curRoom == nullptr) continue;

			curRoom->knight = knight;
			curRoom->createMap();

			memcpy(curRoom->visDirCpy, curRoom->visDir, sizeof(curRoom->visDir));
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
	INT32 additionalRoom = 0;

	if (battleSceneNumber % 5 != 1 && battleSceneNumber % 5 != 2) {
		INT32 temp = battleSceneNumber % 5 ? battleSceneNumber % 5 : 4;
		additionalRoom = temp - 2;
	} //关卡数靠后增加额外房间 暂时最多8个房间 房间太多可能会卡

	if (cntRoom >= MAXROOM + additionalRoom) return;

	srand(static_cast<unsigned int>(time(nullptr)));

	vector<INT32> vecDir;

	for (INT32 dir = 0; dir < CNTDIR; dir++) {
		if (curRoom->visDir[dir]) continue;

		INT32 toX = x + DIRX[dir], toY = y + DIRY[dir];
		if (toX < 0 || toX >= SIZEMTX || toY < 0 || toY >= SIZEMTX) continue;

		if (curRoom == beginRoom &&
			((curRoom->y < 2 && dir == DOWN) || (curRoom->y > 2 && dir == UP)))
			continue;

		vecDir.push_back(dir);  // directions can go
	}

	if (vecDir.empty()) return;

	INT32 cntDirChosen = max(2U, (rand() % (vecDir.size() + 1U)));

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

void BattleScene::setRoomType() {
	Vector<BattleRoom*> vecRoom;           //用于确定特殊房间
	INT32 cntDirEndRoom = 0;
	for (INT32 dir = 0; dir < CNTDIR; dir++)
		if (endRoom->visDir[dir] == true) cntDirEndRoom++;

	for (INT32 y = 0; y < SIZEMTX; y++) {  //判断房间类型并生成地图
		for (INT32 x = 0; x < SIZEMTX; x++) {
			BattleRoom* curRoom = battleRoom[x][y];
			if (curRoom == nullptr) continue;

			if (curRoom == beginRoom)
				curRoom->roomType = BEGIN;
			else if (curRoom == endRoom) {
				if (battleSceneNumber % 5 == 0) //终点为boss房间
					curRoom->roomType = BOSS;
				else
					curRoom->roomType = END;
			}
			else
				curRoom->roomType = NORMAL;

			bool notConnectedToBegin = true;
			//判断一个房间是否与起点相连 相连则设为普通房间 否则待随机选择
			for (INT32 dir = 0; dir < CNTDIR; dir++) {
				if (curRoom == beginRoom || curRoom == endRoom) {
					notConnectedToBegin = false;
					break;
				}
				if (curRoom->visDir[dir] == false) continue;

				BattleRoom* toRoom = battleRoom[x + DIRX[dir]][y + DIRY[dir]];
				if (toRoom == beginRoom)
					notConnectedToBegin = false;  //与起始房间和终点相连
			}
			if (notConnectedToBegin) vecRoom.pushBack(curRoom);
		}
	}

	log("room size %d", vecRoom.size());
	for (INT32 i = 0, roomIndex = 0; i < 2; i++) {  //分别设置 武器 道具
		if (vecRoom.size() == 0) break;

		roomIndex = rand() % vecRoom.size(); //随机选一个房间
		BattleRoom* curRoom = vecRoom.at(roomIndex);
		vecRoom.erase(vecRoom.begin() + roomIndex);
		if (i == 0)
			curRoom->roomType = WEAPON;
		else if (i == 1)
			curRoom->roomType = PROP;
	}
}

void BattleScene::initMiniMap() { //初始化小地图
	for (int x = 0; x < SIZEMTX; x++) {
		for (int y = 0; y < SIZEMTX; y++) {
			if (battleRoom[x][y] == nullptr) continue;
			Color4F color; //根据房间类型选择小地图颜色
			if (battleRoom[x][y]->roomType == BEGIN)
				color = Color4F(.0f, 0.9f, .0f, 0.5f);
			else if (battleRoom[x][y]->roomType == END)
				color = Color4F(.0f, .0f, 1.0f, 0.7f);
			else if (battleRoom[x][y]->roomType == NORMAL)
				color = Color4F(0.0f, 0.0f, 0.0f, 0.75f);
			else if (battleRoom[x][y]->roomType == BOSS)
				color = Color4F(0.9f, 0.0f, 0.0f, 0.5f);
			else if (battleRoom[x][y]->roomType == PROP)
				color = Color4F(1.0f, 0.8f, 0.0f, 0.5f);
			else if (battleRoom[x][y]->roomType == WEAPON)
				color = Color4F(1.0f, 0.5f, 0.0f, 0.5f);

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
		if (curRoom->visDirCpy[dir] == false) continue;
		INT32 toX = curRoom->x + DIRX[dir];
		INT32 toY = curRoom->y + DIRY[dir];

		auto toRoom = battleRoom[toX][toY];
		auto curMiniRoom = miniMap->miniRoom[curRoom->x][curRoom->y];
		auto toMiniRoom = miniMap->miniRoom[toX][toY];

		auto miniHall = DrawNode::create(); //生成小地图走廊
		miniHall->setGlobalZOrder(TOP);

#ifndef DEBUG
		miniHall->setVisible(false);
#endif  // ! 

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

		curRoom->visDirCpy[dir] = false;  //标记不能来回连接
		curMiniRoom->miniHall[dir] = miniHall;
		toRoom->visDirCpy[(dir + 2) % CNTDIR] = false;  //标记不能来回连接
		toMiniRoom->miniHall[(dir + 2) % CNTDIR] = miniHall;

		connectRoom(toRoom);
		// need to fix vis
	}
}

/*进入设置面板*/
void BattleScene::menuCloseCallbackSet(Ref* pSender)
{
	CCScheduler* defaultScheduler = CCDirector::sharedDirector()->getScheduler();
	defaultScheduler->pauseTarget(this);
	Director::getInstance()->pushScene(TransitionFade::create(1.0f, SetScene::createScene()));
	defaultScheduler->resumeTarget(this);
}