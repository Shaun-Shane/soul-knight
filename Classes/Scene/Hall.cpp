#include "Hall.h"
#include "Attack/Bullet.h"
#include "BattleScene.h"

void Hall::update(float delta) { this->bulletMove(); }

Vector<Bullet*>& Hall::getVecPlayerBullet() { return vecPlayerBullet; }

Knight* Hall::getKnight() { return knight; }

Vector<Bullet*>& Hall::getVecEnemyBullet() { return vecEnemyBullet; }

Point Hall::getUpleftVertex() const { return Point(upLeftX, upLeftY); }

Point Hall::getDownRightVertex() const { return Point(downRightX, downRightY); }

bool Hall::init() {
	upLeftX = .0f, upLeftY = .0f;
	downRightX = .0f, downRightY = .0f;

	sizeHeight = SIZEHALL, sizeWidth = SIZEHALL;

	this->scheduleUpdate();
	return true;
}

void Hall::generateFloor(float X, float Y, INT32 layer) {
	INT32 randomNum = rand();

	Sprite* tmpSprite = nullptr;

	INT32 sceneTypeIndex = BattleScene::getSceneNumber();
	sceneTypeIndex =
		sceneTypeIndex % 5 == 0 ? sceneTypeIndex / 5 : sceneTypeIndex / 5 + 1;
	(sceneTypeIndex -= 1) %= BattleScene::getVecSceneType().size();
	Value imageName = Value(BattleScene::getVecSceneType().at(sceneTypeIndex));
	//选取场景类型

	if (randomNum % 4 == 0)
		imageName = "Map//" + imageName.asString() + "floor3.png";
	else if (randomNum % 3 == 0)
		imageName = "Map//" + imageName.asString() + "floor2.png";
	else
		imageName = "Map//" + imageName.asString() + "floor1.png";

	tmpSprite = Sprite::create(imageName.asString().c_str());
	this->addChild(tmpSprite);
	tmpSprite->setGlobalZOrder(layer);
	tmpSprite->setPosition(Point(X, Y));
	vecFloor.pushBack(tmpSprite);
}

void Hall::generateWall(float X, float Y, INT32 layer, bool addShadow) {
	INT32 randomNum = rand();

	Sprite* tmpSprite = nullptr;
	INT32 sceneTypeIndex = BattleScene::getSceneNumber();
	sceneTypeIndex =
		sceneTypeIndex % 5 == 0 ? sceneTypeIndex / 5 : sceneTypeIndex / 5 + 1;
	(sceneTypeIndex -= 1) %= BattleScene::getVecSceneType().size();
	Value imageName = Value(BattleScene::getVecSceneType().at(sceneTypeIndex));
	//选取场景类型

	if (randomNum % 6 == 0)
		imageName = "Map//" + imageName.asString() + "wall2.png";
	else
		imageName = "Map//" + imageName.asString() + "wall1.png";

	tmpSprite = Sprite::create(imageName.asString().c_str(), Rect(0, 0, 40, 35));
	this->addChild(tmpSprite, layer);
	tmpSprite->setGlobalZOrder(layer);
	tmpSprite->setPosition(Point(X, Y + (WALLHEIGHT - FLOORHEIGHT)));
	vecWall.pushBack(tmpSprite);
	// Upside of whe wall

	tmpSprite = Sprite::create(imageName.asString().c_str(), Rect(0, 35, 40, 25));
	this->addChild(tmpSprite, LayerPlayer - 2);
	tmpSprite->setGlobalZOrder(LayerPlayer - 2);
	tmpSprite->setPosition(Point(X, Y + (WALLHEIGHT - FLOORHEIGHT) - 30));
	vecWall.pushBack(tmpSprite);
	// downside of the wall

	if (addShadow) {  //添加阴影
		auto shadow = Sprite::create("Map//RectShadow.png");
		shadow->setGlobalZOrder(LayerPlayer - 1);
		shadow->setPosition(Point(20, -8));
		shadow->setOpacity(140);
		tmpSprite->addChild(shadow);
	}
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
					generateWall(curX, curY, LayerPlayer + 1, false);
				else
					generateWall(curX, curY, LayerPlayer - 1, true);
			}
			else if ((dir % 2 == 1) && (W == 0 || W == sizeWidth - 1)) {
				generateWall(curX, curY, LayerPlayer + 1, false);
			}
			else {
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
		if (knightX >= upLeftX - FLOORWIDTH && knightX <= downRightX + FLOORWIDTH &&
			knightY <= upLeftY + FLOORHEIGHT + 30 &&
			knightY >= downRightY - FLOORHEIGHT - 15) {
			if (ispeedX > 0 && knightX >= downRightX)
				ispeedX = .0f;
			else if (ispeedX < 0 && knightX <= upLeftX)
				ispeedX = .0f;
		}
	}
	else {
		if (knightX >= upLeftX - FLOORWIDTH - 30 &&
			knightX <= downRightX + FLOORWIDTH + 30 &&
			knightY <= upLeftY + FLOORHEIGHT &&
			knightY >= downRightY - FLOORHEIGHT) {
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

bool Hall::isInRange(Vec2 pos) {
	return (upLeftX - FLOORWIDTH / 4 <= pos.x &&
		pos.x <= downRightX + FLOORWIDTH / 4 &&
		downRightY - FLOORHEIGHT / 4 <= pos.y &&
		pos.y <= upLeftY + FLOORHEIGHT / 4);
}

void Hall::bulletMove() {
	for (INT32 i = 0; i < vecPlayerBullet.size(); ++i) {
		auto bullet = vecPlayerBullet.at(i);
		Vec2 pos = bullet->getPosition();
		pos = pos + bullet->getBulletSpeed();
		bullet->setPosition(pos);
		if (this->isInRange(pos) == false) {
			bullet->showEffect(pos, this);
			bullet->removeFromParent();
			vecPlayerBullet.eraseObject(bullet);
			--i;
		}
	}
	for (INT32 i = 0; i < vecEnemyBullet.size(); ++i) {
		auto bullet = vecEnemyBullet.at(i);
		Vec2 pos = bullet->getPosition();
		pos = pos + bullet->getBulletSpeed();
		bullet->setPosition(pos);
		if (this->isInRange(pos) == false) {
			bullet->showEffect(pos, this);
			bullet->removeFromParent();
			vecEnemyBullet.eraseObject(bullet);
			--i;
		}
	}
}