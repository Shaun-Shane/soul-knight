#include "BattleRoom.h"
#include "BattleScene.h"
#include "Props/prop.h"
#include "Map/Statue.h"


void BattleRoom::setCenter(float X, float Y) { centerX = X, centerY = Y; }

Vector<Sprite*>& BattleRoom::getVecBox() { return this->vecBox; }

void BattleRoom::checkObstacle(Entity* entity) {     }

Vector<Enemy*>& BattleRoom::getVecEnemy() { return vecEnemy; }

Vector<Prop*>& BattleRoom::getVecProps() { return vecProps; }

Vector<Weapon*>& BattleRoom::getVecWeapon() { return vecWeapon; }

Boss* BattleRoom::getBoss() { return boss; }

Statue* BattleRoom::getStatue() { return statue; }

Sprite* BattleRoom::getPortal() { return portal; }

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
	boss = nullptr, statue = nullptr;
	portalTextLabel = nullptr;

	this->scheduleUpdate();
	return true;
}

void BattleRoom::update(float delta) {
	this->checkStatue(); //雕像碰撞检测
	this->checkPortal(); //检测传送门
	this->bulletMove();
	this->bulletCollistionCheck();
	this->removeKilledEnemy(); //移除血量<=0的敌人
}

bool BattleRoom::createRoom(BattleRoom*& toRoom, BattleRoom* curRoom, INT32 dir,
	INT32 toX, INT32 toY) {
	if (toRoom != nullptr) {
		// room was built, no not need to build again
		curRoom->visDir[dir] = true;
		toRoom->visDir[(dir + 2) % CNTDIR] = true;
		// just connect them if toRoom is not beginRoom
		return false;
	}

	toRoom = BattleRoom::create();

	toRoom->x = toX, toRoom->y = toY;
	log("%d %d", toX, toY);
	toRoom->setCenter(curRoom->centerX + DIRX[dir] * CENTERDIS,
		curRoom->centerY + DIRY[dir] * CENTERDIS);

	curRoom->visDir[dir] = true;
	toRoom->visDir[(dir + 2) % CNTDIR] = true;
	return true;
}

void BattleRoom::generateDoor(float X, float Y, INT32 layer) {
	Sprite* tmpSprite = Sprite::create("Map//doorOpen.png");
	this->addChild(tmpSprite, LayerPlayer - 2);
	tmpSprite->setGlobalZOrder(LayerPlayer - 2);
	vecDoorOpen.pushBack(tmpSprite);

	tmpSprite->setPosition(Point(X, Y));
	tmpSprite->setVisible(true);

	tmpSprite = Sprite::create("Map//doorClose.png");
	this->addChild(tmpSprite, layer);
	tmpSprite->setGlobalZOrder(layer);
	vecDoorClose.pushBack(tmpSprite);

	tmpSprite->setPosition(Point(X, Y + FLOORHEIGHT / 2));
	tmpSprite->setVisible(false);  // closeDoor images are not visible at first

	auto shadow = Sprite::create("Map//RectShadow.png");
	shadow->setGlobalZOrder(LayerPlayer - 1);
	shadow->setPosition(Point(20, -8));
	shadow->setOpacity(140);
	tmpSprite->addChild(shadow);
	//添加阴影
}

void BattleRoom::generateStatue() {
	auto statue = Statue::create();

	statue->setPosition(Point(centerX, centerY + 50));
	this->addChild(statue);
	this->statue = statue;
}

void BattleRoom::createMap() {
	srand(time(nullptr));

	if (roomType == END || roomType == WEAPON || roomType == PROP) {
		sizeWidth -= 6, sizeHeight -= 6;

		if (roomType == END) {  //设置房间大小以及传送门
			sizeWidth -= 2, sizeHeight -= 2;
			Sprite* portal = Sprite::create("Map//portal3.png");
			portal->setPosition(Point(centerX, centerY));
			addChild(portal);
			portal->setGlobalZOrder(LayerPlayer - 1);

			this->portal = portal;
		}
	}
	else if (roomType == BOSS) {
		sizeWidth += 6, sizeHeight += 6;
	}

	addMapElement();  //添加地图元素: 地板 墙 门

	if (roomType == PROP) generateStatue();
	if (roomType == WEAPON) {
		createBox((upLeftX + downRightX) / 2, (upLeftY + downRightY) / 2);

	}
}

void BattleRoom::addMapElement() {
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
				}
				else if (H != sizeHeight - 1 || W == 0 || W == sizeWidth - 1) {
					if (H == sizeHeight / 2 + SIZEHALL - 4)
						generateWall(curX, curY, LayerPlayer + 1, true);
					else
						generateWall(curX, curY, LayerPlayer + 1, false);
				}
				else if (visDir[UP] && H == sizeHeight - 1 &&
					(W == sizeWidth / 2 - 3 ||
						W == sizeWidth / 2 + SIZEHALL - 4)) {
					generateWall(curX, curY, LayerPlayer + 2, true);
				}
				else {
					if (H == sizeHeight - 1)  //上方的墙添加阴影
						generateWall(curX, curY, LayerPlayer - 1, true);
					else
						generateWall(curX, curY, LayerPlayer - 1, false);
				}
			}
			else {
				generateFloor(curX, curY, LayerPlayer - 2);
			}  // randomly generate floor and Wall

			curX += FLOORWIDTH;
		}
		curX = X, curY -= FLOORHEIGHT;
	}
}

void BattleRoom::createEnemy() {
	srand(static_cast<unsigned int>(time(nullptr)));

	INT32 sceneTypeIndex = BattleScene::getSceneNumber();
	sceneTypeIndex =
		sceneTypeIndex % 5 == 0 ? sceneTypeIndex / 5 : sceneTypeIndex / 5 + 1;
	INT32 enemyNumber = 13 + rand() % 4 + (sceneTypeIndex - 1) * 2;  //敌人数量

	(sceneTypeIndex -= 1) %= BattleScene::getVecSceneType().size();
	Value sceneName = Value(BattleScene::getVecSceneType().at(sceneTypeIndex));
	//选取场景类型
#ifdef YYZ_DEBUG
	sceneName = Value("Forest//");
#endif

	for (INT32 i = 1; i <= enemyNumber; i++) {
		Enemy* enemy = Enemy::create();
		enemy->bindAtBattleRoom(this); //绑定所在房间
		enemy->startCount = i * 2;

		INT32 enemyType = rand();
		enemyType = (enemyType && enemyType % 4 == 0) ? 3 : enemyType % 4;

		switch (enemyType) {
		case 0:
			enemy->bindSprite(
				Sprite::create("Enemy//" + sceneName.asString() + "enemy002.png"),
				LayerPlayer - 1);
			break;
		case 1:
			enemy->bindSprite(
				Sprite::create("Enemy//" + sceneName.asString() + "enemy007.png"),
				LayerPlayer - 1);
			break;
		case 2:
			enemy->bindSprite(
				Sprite::create("Enemy//" + sceneName.asString() + "enemy001.png"),
				LayerPlayer - 1);
			break;
		case 3:
			enemy->bindSprite(
				Sprite::create("Enemy//" + sceneName.asString() + "enemy003.png"),
				LayerPlayer - 1);
			break;
		}

		enemy->setType(enemyType);
		enemy->addShadow(Point(enemy->getContentSize().width / 2.3f,
			enemy->getContentSize().height / 9),
			LayerPlayer - 1);  //添加阴影
		vecEnemy.pushBack(enemy);
	}

	INT32 addChildNum =
		std::min(static_cast<INT32>(vecEnemy.size()), 4 + rand() % 3);
	for (auto it = vecEnemy.rbegin(); it != vecEnemy.rbegin() + addChildNum;
		it++) {
		float enemyX = centerX + (rand() * 2 - RAND_MAX) % 300;
		float enemyY = centerY + (rand() * 2 - RAND_MAX) % 300;

		(*it)->setPosition(Point(enemyX, enemyY));
		(*it)->setIsAdded(true);
		this->addChild(*it);  //分批次添加到场景
	}
}

void BattleRoom::createBoss() {
	boss = Boss::create();
	boss->bindAtBattleRoom(this);

	boss->bindSprite(Sprite::create("Enemy//bossWithoutSword.png"), LayerPlayer - 1);
	auto swordSprite = Sprite::create("Enemy//swordOfBoss.png");
	boss->createSword(swordSprite);
	boss->addShadow(Point(boss->getContentSize().width / 2,
		boss->getContentSize().height / 4.5f),
		LayerPlayer);  //添加阴影

	boss->setPosition(Point(centerX, centerY));
	this->addChild(boss);
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
		if (allKilled()) {
			openDoor();
			if (roomType == BEGIN)
				knight->setNeedCreateBox(false);
			else {
				if (knight->getNeedCreateBox() == true) {
					INT32 curMP = this->knight->getMP() + 20;
					this->knight->setMP(curMP); //setMp会判断是否超限
					float x, y;
					x = (upLeftX + downRightX) / 2;
					y = (upLeftY + downRightY) / 2;
					if (roomType == BOSS) {
						y -= 150;
					}
					createBox(x, y);
					knight->setNeedCreateBox(false);
				}
			}
		}
		else {
			if (knight->getNeedCreateBox() == false) knight->setNeedCreateBox(true);
			closeDoor();
		}

		if (!allKilled()) {
			if (ispeedX > 0 && knightX >= downRightX) ispeedX = .0f;
			if (ispeedX < 0 && knightX <= upLeftX) ispeedX = .0f;
			if (ispeedY > 0 && knightY >= upLeftY + 20) ispeedY = .0f;
			if (ispeedY < 0 && knightY <= downRightY) ispeedY = .0f;
		}
		else {
			if (((upLeftY + FLOORHEIGHT / 2 - FLOORHEIGHT * (sizeHeight / 2 - 3)) >=
				knightY &&
				knightY >= (downRightY + FLOORHEIGHT * (sizeHeight / 2 - 3)))) {
				if (ispeedX > 0 && knightX >= downRightX && !visDir[RIGHT])
					ispeedX = .0f;
				if (ispeedX < 0 && knightX <= upLeftX && !visDir[LEFT]) ispeedX = 0.f;
			}
			else if (upLeftX + FLOORHEIGHT * (sizeHeight / 2 - 3) <= knightX &&
				knightX <= downRightX - FLOORHEIGHT * (sizeHeight / 2 - 3)) {
				if (ispeedY > 0 && knightY >= upLeftY + FLOORHEIGHT / 2 && !visDir[UP])
					ispeedY = .0f;
				if (ispeedY < 0 && knightY <= downRightY && !visDir[DOWN])
					ispeedY = 0.f;
			}
			else {
				if (ispeedX > 0 && knightX >= downRightX) ispeedX = .0f;
				if (ispeedX < 0 && knightX <= upLeftX) ispeedX = .0f;
				if (ispeedY > 0 && knightY >= upLeftY + 20) ispeedY = .0f;
				if (ispeedY < 0 && knightY <= downRightY) ispeedY = .0f;
			}
		}

		return true;
	}
	return false;
}

void BattleRoom::createBox(float x, float y)
{
	Sprite* box;
	if (roomType == WEAPON) box = Sprite::create("Box//box1.png");
	else  box = Sprite::create("Box//box2.png");
	box->setPosition(Vec2(x, y));
	box->setGlobalZOrder(LayerPlayer - 2);
	this->addChild(box, TOP);
	this->getVecBox().pushBack(box);
}

void BattleRoom::bulletCollistionCheck() {
	for (INT32 i = 0; i < vecPlayerBullet.size(); ++i) {
		auto bullet = vecPlayerBullet.at(i);
		Rect bulletRect = bullet->getBoundingBox();

		if (statue != nullptr) {
			Rect statueRect = statue->getBoundingBox();  //检测雕像 之后可能加障碍物
			if (statueRect.containsPoint(bullet->getPosition())) {
				bullet->showEffect(bullet->getPosition(), this);  //子弹击中特效
				bullet->removeFromParent();
				vecPlayerBullet.eraseObject(bullet);
				--i;
				continue;
			}
		}

		if (getBoss() != nullptr && getBoss()->getIsKilled() == false) {
			Rect bossRect = getBoss()->getBoundingBox();
			if (bulletRect.intersectsRect(bossRect)) {
				getBoss()->deductHP(bullet->getAttack() * knight->getDamageBuff());
				bullet->showEffect(bullet->getPosition(), this); //子弹击中特效
				bullet->removeFromParent();
				vecPlayerBullet.eraseObject(bullet);
				--i;
			}
		}
		else {
			for (INT32 j = 0; j < vecEnemy.size(); ++j) { //检测怪物
				auto enemy = vecEnemy.at(j);
				if (enemy->getParent() == nullptr || enemy->getIsKilled()) continue;
				Rect enemyRect = enemy->getBoundingBox();
				if (bulletRect.intersectsRect(enemyRect)) {
					INT32 hp = knight->getHP();
					enemy->deductHP(bullet->getAttack() * knight->getDamageBuff());

					bullet->showEffect(bullet->getPosition(), this); //子弹击中特效
					bullet->removeFromParent();
					vecPlayerBullet.eraseObject(bullet);
					--i;
					break;
				}
			}
		}
	}
	for (INT32 i = 0; i < vecEnemyBullet.size(); ++i) {
		auto bullet = vecEnemyBullet.at(i);
		Rect bulletRect = bullet->getBoundingBox();
		Rect knightRect = knight->getBoundingBox();
		if (bulletRect.intersectsRect(knightRect)) {
			knight->deductHP(bullet->getAttack());
			bullet->showEffect(bullet->getPosition(), this); //子弹击中特效
			bullet->removeFromParent();
			vecEnemyBullet.eraseObject(bullet);
			--i;
		}
	}
}

void BattleRoom::checkStatue() { //检测雕像障碍物
	if (this->roomType != PROP) return;
	float knightX = knight->getPositionX();
	float knightY = knight->getPositionY();
	//障碍物检测
	if (knightX <= centerX + 90 && knightX >= centerX - 90 && knightY <= centerY + 90 && knightY >= centerY - 90) {
		if (centerY - 40 < knightY && knightY < centerY + 70) {
			if (knightX < centerX && knightX >= centerX - 60 &&
				knight->getMoveSpeedX() > 0)
				knight->setMoveSpeedX(.0f);
			else if (knightX > centerX && knightX <= centerX + 60 &&
				knight->getMoveSpeedX() < 0)
				knight->setMoveSpeedX(.0f);
		} //no else
		if (centerX - 50 < knightX && knightX < centerX + 50) {
			if (knightY < centerY && knightY >= centerY - 50 &&
				knight->getMoveSpeedY() > 0)
				knight->setMoveSpeedY(.0f);
			else if (knightY > centerY && knightY <= centerY + 80 &&
				knight->getMoveSpeedY() < 0)
				knight->setMoveSpeedY(.0f);
			//根据人物位置 访问信息显示雕像标签
			if (centerX - 40 < knightX && knightX < centerX + 40) {
				if (knightY < centerY && knightY >= centerY - 70 && !statue->getPlayerVisited())
					statue->getTextLabel()->setVisible(true);
				else
					statue->getTextLabel()->setVisible(false);
			}
			else {
				statue->getTextLabel()->setVisible(false);
			}
		}
	}
}

void BattleRoom::checkPortal() {
	if (portal == nullptr) return;

	if (portalTextLabel == nullptr) {
		portalTextLabel =
			Label::create("Portal--Press J", "fonts/Marker Felt.ttf", 22);
		this->addChild(portalTextLabel);
		portalTextLabel->setPosition(Point(centerX, centerY + 100));
		portalTextLabel->setGlobalZOrder(TOP);
		portalTextLabel->setVisible(false);
	}
	//根据骑士位置选择是否隐藏标签
	if (portal->getPosition().getDistance(knight->getPosition()) < 30.0f) {
		portalTextLabel->setVisible(true);
	}
	else {
		portalTextLabel->setVisible(false);
	}
}

void BattleRoom::removeKilledEnemy() {
	for (auto& e : vecEnemy) {
		if (e->getParent() == nullptr || e->getIsKilled()) continue;
		if (e->getHP() <= 0) { //血量小于零 移除
			e->setIsKilled(true);
			e->showDeathEffect(); //死亡特效
			int digit = rand() % 10;
			if (digit >= 4) this->knight->addGold(1);
		}
	}

	if (boss != nullptr && boss->getParent() != nullptr && boss->getIsKilled() == false) {
		if (boss->getHP() <= 0) { // 血量小于零 移除
			boss->setIsKilled(true);
			boss->showDeathEffect(); //死亡特效
			this->knight->addGold(5);
		}
	}
}

bool BattleRoom::allKilled() {
	bool allKilled = true;
	for (auto e : vecEnemy) {
		if (e->getIsKilled() == false || e->getParent() != nullptr) allKilled = false;
	}

	if (boss != nullptr && boss->getIsKilled() == false) allKilled = false;

	return allKilled;
}

void BattleRoom::openTreasureBox() {
	srand(time(NULL));
	int randomDigit;
	if (roomType == WEAPON) randomDigit = rand() % 4;
	else  //设置出物品概率   40%啥都没有，10%出枪，10%出加血，10%出加蓝，10出加金币，20出加护具
	{
		int randNun = rand() % 10;
		if (randNun == 0) randomDigit = rand() % 4;
		else if (randNun == 1)  randomDigit = 4;
		else if (randNun == 2)  randomDigit = 5;
		else if (randNun == 3)  randomDigit = 6;
		else if (randNun == 4 || randNun == 5) randomDigit = 7;
		else randomDigit = 8;

	}
	if (randomDigit <= 3)
		crearteWeapon(randomDigit);
	else
		createProps(randomDigit);
}

void BattleRoom::crearteWeapon(int randomDigit) {
	Weapon* weapon = Weapon::create();
	switch (randomDigit) {
	case 0:
		//以下参数含义以此为 speed,  attack,  decMP,  weaponType,  state,  bulletType
		weapon->weaponInit(25.0f, 2, 1, 2, true, 12);
		break;
	case 1:
		weapon->weaponInit(23.0f, 4, 3, 3, true, 13);
		break;
	case 2:
		weapon->weaponInit(24.0f, 6, 4, 4, true, 14);
		break;
	case 3:
		weapon->weaponInit(0.0f, 7, 0, 5, false, 15);
		break;
	}
	weapon->setPosition(
		Vec2((upLeftX + downRightX) / 2, (upLeftY + downRightY) / 2));
	this->addChild(weapon, TOP);
	this->getVecWeapon().pushBack(weapon);
	int p = getVecWeapon().size();
	CCLOG("vecWeapon Size:%d", getVecWeapon().size());
}

void BattleRoom::createProps(int randomDigit) {
	Prop* props = Prop::create();
	switch (randomDigit) {
	case 4:
		props->bindSprite(Sprite::create("Props//add_HP.png"), TOP);
		props->setPropIndex(1);
		break;
	case 5:
		props->bindSprite(Sprite::create("Props//add_MP.png"), TOP);
		props->setPropIndex(2);
		break;
	case 6:
		props->bindSprite(Sprite::create("Props//add_gold.png"), TOP);
		props->setPropIndex(3);
		break;
	case 7:
		props->bindSprite(Sprite::create("Props//add_protect.png"), TOP);
		props->setPropIndex(4);
		break;
	case 8:
		return;
	}
	props->setPosition(
		Vec2((upLeftX + downRightX) / 2, (upLeftY + downRightY) / 2));
	this->addChild(props, TOP);
	this->getVecProps().pushBack(props);
}

