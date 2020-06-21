#include "Knight.h"
#include "Attack/Weapon.h"
#include "FlowWord.h"
#include "Map/Statue.h"
#include "Scene/BattleRoom.h"
#include "Scene/Hall.h"
#include "SimpleAudioEngine.h"

Knight::~Knight() {}

void Knight::bindBattleRoom(BattleRoom* battleRoom) { atBattleRoom = battleRoom; }

void Knight::bindHall(Hall* hall) { atHall = hall; }

void Knight::setNeedCreateBox(bool need) { this->needCreateBox = need; }

bool Knight::getNeedCreateBox() { return this->needCreateBox; }

INT32 Knight::getMP() const { return this->MP; }

void Knight::setMP(INT32 mp) { this->MP = std::min(maxMP, std::max(0, mp)); }

INT32 Knight::getMaxMP() const { return this->maxMP; }

void Knight::setMaxMP(INT32 maxMP) { this->maxMP = maxMP; }

INT32 Knight::getArmor() const { return this->armor; }

void Knight::setArmor(INT32 armor) { this->armor = std::min(maxArmor, armor); }

INT32 Knight::getMaxArmor() const { return this->maxArmor; }

void Knight::setMaxArmor(INT32 maxArmor) { this->maxArmor = maxArmor; }

INT32 Knight::getDamageBuff() const { return this->damageBuff; }

void Knight::setDamageBuff(INT32 damageBuff) { this->damageBuff = damageBuff; }

INT32 Knight::getMoveSpeedBuff() const { return this->moveSpeedBuff; }

void Knight::setMoveSpeedBuff(INT32 msBuff) { this->moveSpeedBuff = std::min(3, msBuff); }

BattleRoom* Knight::getAtBattleRoom() { return this->atBattleRoom; }

Hall* Knight::getAtHall() { return atHall; }

void Knight::addGold(INT32 deta) { (this->gold) += deta; }


Animate* Knight::getAnimate() {
	//创建序列帧动画
	auto animation = Animation::create();

	//设置动画名字数组的长度
	char nameSize[30] = { 0 };

	//动画的循环4张图片
	for (INT32 i = 1; i < 5; i++) {
		sprintf(nameSize, "Character//Knight%d.png", i);

		//添加到序列帧动画
		animation->addSpriteFrameWithFile(nameSize);
	}
	//设置动画帧的时间间隔
	animation->setDelayPerUnit(0.1f);

	//设置播放循环 一直播放 为-1
	animation->setLoops(-1);

	//设置动画结束后恢复到第一帧
	animation->setRestoreOriginalFrame(true);

	//创建动画动作
	auto animate = Animate::create(animation);
	return animate;
}

bool Knight::init() {
	this->HP = this->maxHP = 5;
	this->armor = this->maxArmor = 5;
	this->MP = this->maxMP = 200;
	this->gold = 0;
	this->moveSpeedX = 0, this->moveSpeedY = 0;
	this->damageBuff = 1, this->moveSpeedBuff = 0;
	this->ultimateSkillTime = ultimateSkillGap;

	this->weapon = Weapon::create();
	//float speed, INT32 weaponAttack, INT32 decMP, int weaponType, bool state, int bulletType
	this->weapon->weaponInit(24.0f, 2, 0, 1, true, 11);
	this->weapon->setPosition(Vec2(40, 20));
	this->addChild(weapon, TOP);

	this->attackCount = 0;

	isInvincible = false, goIntoPortal = false;

	registerKeyboardEvent();

	this->scheduleUpdate();
	return true;
}

void Knight::registerKeyboardEvent() {
	auto listener = EventListenerKeyboard::create();

	listener->onKeyPressed = [&](EventKeyboard::KeyCode code, Event*) {
		static Vec2 last;
		static bool isRight = true;
		if (code != EventKeyboard::KeyCode::KEY_D &&
			code != EventKeyboard::KeyCode::KEY_W &&
			code != EventKeyboard::KeyCode::KEY_A &&
			code != EventKeyboard::KeyCode::KEY_S &&
			code != EventKeyboard::KeyCode::KEY_J &&
			code != EventKeyboard::KeyCode::KEY_K)
			last.set(1.0, 0);

		switch (code) {
		case EventKeyboard::KeyCode::KEY_D:
			last.set(1.0, 0);
			isRight = true;
			moveSpeedX = moveSpeed;
			getSprite()->setFlippedX(false);
			weapon->getSprite()->setFlippedX(false);
			if (!isAnimation) {
				getSprite()->runAction(getAnimate());
				isAnimation = true;
			}
			break;

		case EventKeyboard::KeyCode::KEY_W:
			last.set(0, 1.0);
			moveSpeedY = moveSpeed;
			if (!isAnimation) {
				getSprite()->runAction(getAnimate());  //执行帧动作
				isAnimation = true;
			}
			break;

		case EventKeyboard::KeyCode::KEY_A:
			isRight = false;
			last.set(-1.0, 0);
			moveSpeedX = -moveSpeed;
			getSprite()->setFlippedX(true);
			weapon->getSprite()->setFlippedX(true);
			if (!isAnimation) {
				getSprite()->runAction(getAnimate());
				isAnimation = true;
			}
			break;

		case EventKeyboard::KeyCode::KEY_S:
			last.set(0, -1.0);
			moveSpeedY = -moveSpeed;
			if (!isAnimation) {
				getSprite()->runAction(getAnimate());
				isAnimation = true;
			}
			break;

		case EventKeyboard::KeyCode::KEY_J:
			if (this->atHall == nullptr && this->atBattleRoom == nullptr) break;

			if (checkPortal()) break;
			if (checkStatue()) break;

			if (this->atBattleRoom != nullptr) {
				Weapon* weaponCheck = this->collisionWithWeaponCheck();
				Prop* prop = this->collisionWithCropCheck();
				Sprite* box = this->collisionWithBoxCheck();
				Enemy* enemy = this->collisionWithEnemyCheck();
				Boss* boss = this->collisionWithBossCheck();
				if (weaponCheck != nullptr) {
					this->bindWeapon(weaponCheck);
					if (isRight == false) weapon->getSprite()->setFlippedX(true);
					break;
				}
				else if (prop != nullptr) {
					prop->useProps(this);
					prop->removeFromParent();
					this->atBattleRoom->getVecProps().eraseObject(prop);
					break;
				}
				else if (box != nullptr) {
					this->getAtBattleRoom()->openTreasureBox();
					box->removeFromParent();
					this->atBattleRoom->getVecBox().eraseObject(box);
					break;
				}
				else if (enemy != nullptr)
					enemy->deductHP(5);
				else if (boss != nullptr)
					boss->deductHP(10);
			}
			weaponAttack(last);
			break;

		case EventKeyboard::KeyCode::KEY_K:
			if (this->atHall == nullptr && this->atBattleRoom == nullptr) break;
			useUltimateSkill();
			break;
		}
	};

	listener->onKeyReleased = [&](EventKeyboard::KeyCode code, Event*) {
		switch (code) {
		case EventKeyboard::KeyCode::KEY_D:
			moveSpeedX = .0f;
			getSprite()->stopAllActions();  //停止帧动画动作
			isAnimation = false;
			break;

		case EventKeyboard::KeyCode::KEY_W:
			moveSpeedY = .0f;
			getSprite()->stopAllActions();
			isAnimation = false;
			break;

		case EventKeyboard::KeyCode::KEY_A:
			moveSpeedX = .0f;
			getSprite()->stopAllActions();
			isAnimation = false;
			break;

		case EventKeyboard::KeyCode::KEY_S:
			moveSpeedY = .0f;
			getSprite()->stopAllActions();
			isAnimation = false;
			break;
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Knight::useUltimateSkill() {
	if (ultimateSkillTime < ultimateSkillGap) {
		INT32 cdTime = ceil((ultimateSkillGap - ultimateSkillTime) / 60.0f);
		auto textLabel = Label::create(
			"Ultimate Skill in " + Value(cdTime).asString() + " seconds CD!",
			"fonts/Marker Felt.ttf", 20);
		textLabel->setColor(ccc3(255, 255, 255));
		textLabel->setGlobalZOrder(TOP);
		textLabel->setPosition(Point(20, 70));
		this->addChild(textLabel);

		/*创建一个延时动作*/
		auto fadeOut = FadeOut::create(1.6f);

		auto actions = Sequence::create(fadeOut, RemoveSelf::create(), NULL);
		textLabel->runAction(actions);
		return;
	}
	ultimateSkillTime = 0;

	log("using ultimate skill!");
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadEffect("audioEffect//explosion.wav");
	static INT32 temUS = 0;

	audio->stopEffect(temUS);  //暂停之前的音效
	audio->playEffect("audioEffect//explosion.wav", false);

	auto skillCircle = DrawNode::create();
	skillCircle->drawSolidCircle(Point(this->getContentSize().width / 2,
		this->getContentSize().height / 2),
		280.0f, CC_DEGREES_TO_RADIANS(360), 100,
		Color4F(1.0f, 0.8f, .0f, 0.3f));

	skillCircle->setGlobalZOrder(LayerPlayer);

	auto fadeIn = FadeIn::create(0.1f);
	auto fadeOut = FadeOut::create(0.2f);
	auto blink = Blink::create(0.3f, 2);

	auto sequence = Sequence::create(
		Spawn::create(Sequence::create(fadeIn, fadeOut, NULL), blink, NULL),
		RemoveSelf::create(), NULL);  //生成动作序列

	this->addChild(skillCircle);

	skillCircle->runAction(sequence);  //执行动画

	if (this->atBattleRoom == nullptr) return;

	Vector<Enemy*>& vecEnemy = atBattleRoom->getVecEnemy();

	for (auto& e : vecEnemy) {
		if (e->getParent() == nullptr) continue;

		float enemyX = e->getPositionX(), enemyY = e->getPositionY();

		if (sqrt(pow(getPositionX() - enemyX, 2) +
			pow(getPositionY() - enemyY, 2)) <= 280.0f) {
			e->deductHP(20 * damageBuff);  //在技能圆内 扣血
		}
	}

	auto boss = atBattleRoom->getBoss();
	if (boss != nullptr && boss->getParent() != nullptr) {
		float bossX = boss->getPositionX(), bossY = boss->getPositionY();
		if (sqrt(pow(getPositionX() - bossX, 2) + pow(getPositionY() - bossY, 2)) <=
			280.0f) {
			boss->deductHP(20 * damageBuff);  //在技能圆内 扣血
		}
	}

	if (this->atBattleRoom != nullptr) {
		assert(atHall == nullptr);
		if (this->atBattleRoom->allKilled() == true) vecEnemy.clear();
	}
}

void Knight::deductHP(INT32 delta) {
	preAttackedTime = curTime;  //被攻击的时间 用于护甲的恢复判断

	armor -= delta;   //护甲先直接减去扣血值
	if (armor < 0) {  //小于零则HP加上护甲抵消后的扣血值
		HP = std::max(0, HP + armor);
	}
	armor = std::max(0, armor);

	/*受伤特效*/
	FlowWord* flowWord = FlowWord::create();
	this->addChild(flowWord);
	flowWord->showWord(-delta, getSprite()->getPosition() +
		Vec2(0, this->getContentSize().height / 2.3f));
}

void Knight::resumeArmor() {  //恢复护甲
	curTime++;
	if (ultimateSkillTime < ultimateSkillGap) {
		ultimateSkillTime++;
	}

	if (armor == maxArmor) return;
	// 3秒未被攻击 则每隔一秒恢复一护甲
	if (curTime - preAttackedTime >= 180 &&
		(curTime - preAttackedTime) % 55 == 0) {
		armor++;
	}
}

bool Knight::checkStatue() {
	if (this->atBattleRoom == nullptr) return false;

	auto statue = this->atBattleRoom->getStatue();

	if (statue == nullptr) return false;  //没有雕像直接false

	if (statue->getTextLabel()->isVisible()) {
		if (statue->getPlayerVisited() == false) {
			if (this->gold >= statue->getGoldCost()) {
				this->gold -= statue->getGoldCost();
				statue->giveBuff(this);
				statue->setPlayerVisited(true);
				return true;
			}
			else {
				auto textLabel =
					Label::create("Gold is not Enough!", "fonts/Marker Felt.ttf", 20);
				textLabel->setColor(ccc3(255, 255, 255));
				textLabel->setGlobalZOrder(TOP);
				textLabel->setPosition(Point(20, 70));
				this->addChild(textLabel);

				/*创建一个延时动作*/
				auto fadeOut = FadeOut::create(1.2f);

				auto actions = Sequence::create(fadeOut, RemoveSelf::create(), NULL);
				textLabel->runAction(actions);
			}
		}
	}
	return false;
}

bool Knight::checkPortal() {  //检测传送门 按j进入下一关卡
	if (this->atBattleRoom == nullptr) return false;

	auto portal = this->atBattleRoom->getPortal();

	if (portal == nullptr) return false;

	if (portal->getPosition().getDistance(this->getPosition()) < 30.0f) {
		this->goIntoPortal = true;
		return true;
	}
	return false;
}

void Knight::weaponAttack(Vec2 last) {
	if (this->MP <= 0 && this->weapon->getMPConsumption() > 0) return;

	this->setMP(this->getMP() - this->weapon->getMPConsumption());
	if (this->weapon->getWeaponState() == false) {
		//添加音效
		auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
		audio->preloadEffect("audioEffect//bulletEffect.wav");
		static INT32 temKnife = 0;

		audio->stopEffect(temKnife);//暂停之前的音效
		temKnife = audio->playEffect("audioEffect//bulletEffect.wav", false);

		this->weapon->knifeAttack(this);
		return;
	}
	Vec2 fireSpeed = last * (this->weapon->getFireSpeed());
	INT32 firePower = this->weapon->getAttack();
	Vec2 curPos = this->getPosition();
	Vec2 target;
	if (this->atBattleRoom != nullptr) {
		Boss* boss = this->atBattleRoom->getBoss();
		if (boss != nullptr && boss->getIsKilled() == false) {
			target = boss->getPosition() - curPos;
			target.set(target.x / target.length(), target.y / target.length());
			fireSpeed = target * this->weapon->getFireSpeed();
		}
		else {
			Vector<Enemy*>& vecEnemy = atBattleRoom->getVecEnemy();
			Enemy* nearNeast = nullptr;
			float distance = 99999;
			for (auto e : vecEnemy) {
				if (e->getParent() != nullptr && e->getIsKilled() == false) {
					Vec2 enemyPos = e->getPosition();
					if (enemyPos.distance(curPos) < distance) {
						nearNeast = e;
						distance = enemyPos.distance(curPos);
					}
				}
			}
			if (nearNeast != nullptr) {
				target = nearNeast->getPosition() - curPos;
				target.set(target.x / target.length(), target.y / target.length());
				fireSpeed = target * this->weapon->getFireSpeed();
			}
		}
	}

	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadEffect("audioEffect//bulletEffect.mp3");
	static INT32 temBullet = 0;

	attackCount++;
	Bullet* bullet;
	if (attackCount <= 5)  bullet = this->weapon->createBullet(fireSpeed, firePower, false);
	else {
		bullet = this->weapon->createBullet(fireSpeed, firePower, true);
		attackCount = 0;
	}
	bullet->setPosition(curPos);
	audio->stopEffect(temBullet);  //暂停之前的音效
	temBullet = audio->playEffect("audioEffect//bulletEffect.mp3", false);
	(atBattleRoom != nullptr ? atBattleRoom : atHall)->addChild(bullet);
	(atBattleRoom != nullptr ? atBattleRoom : atHall)
		->getVecPlayerBullet()
		.pushBack(bullet);
}

Weapon* Knight::collisionWithWeaponCheck() {
	for (INT32 i = 0; i < this->atBattleRoom->getVecWeapon().size(); ++i) {
		auto weaponPick = this->atBattleRoom->getVecWeapon().at(i);
		Rect weaponRect = weaponPick->getBoundingBox();
		Rect kightRect = this->getBoundingBox();
		if (weaponRect.intersectsRect(kightRect)) return weaponPick;
	}
	return nullptr;
}

Prop* Knight::collisionWithCropCheck() {
	for (INT32 i = 0; i < this->atBattleRoom->getVecProps().size(); ++i) {
		auto prop = this->atBattleRoom->getVecProps().at(i);
		Rect cropRect = prop->getBoundingBox();
		Rect kightRect = this->getBoundingBox();
		if (cropRect.intersectsRect(kightRect)) return prop;
	}
	return nullptr;
}

Enemy* Knight::collisionWithEnemyCheck() {
	for (INT32 i = 0; i < this->atBattleRoom->getVecEnemy().size(); ++i) {
		auto enemy = this->atBattleRoom->getVecEnemy().at(i);
		Rect enemyRect = enemy->getBoundingBox();
		Rect kightRect = this->getBoundingBox();
		if (enemyRect.intersectsRect(kightRect)) return enemy;
	}
	return nullptr;
}

Boss* Knight::collisionWithBossCheck() {
	Boss* boss = this->atBattleRoom->getBoss();
	if (boss == nullptr) return nullptr;
	Rect bossRect = boss->getBoundingBox();
	Rect kightRect = this->getBoundingBox();
	if (bossRect.intersectsRect(kightRect)) return boss;
	return nullptr;
}

Sprite* Knight::collisionWithBoxCheck() {
	for (INT32 i = 0; i < this->atBattleRoom->getVecBox().size(); ++i) {
		auto box = this->atBattleRoom->getVecBox().at(i);
		Rect boxRect = box->getBoundingBox();
		Rect kightRect = this->getBoundingBox();
		if (boxRect.intersectsRect(kightRect)) return box;
	}
	return nullptr;
}

void Knight::bindWeapon(Weapon* weapon) {
	this->atBattleRoom->getVecWeapon().eraseObject(weapon);
	auto myWeapon = this->weapon;
	auto pickWeapon = weapon;
	auto myPos = myWeapon->getPosition();
	auto pickPos = weapon->getPosition();
	myWeapon->setPosition(pickPos);
	pickWeapon->setPosition(myPos);

	myWeapon->retain();
	pickWeapon->retain();

	myWeapon->removeFromParent();
	pickWeapon->removeFromParent();

	this->weapon = pickWeapon;

	this->addChild(pickWeapon);
	this->atBattleRoom->addChild(myWeapon);
	this->atBattleRoom->getVecWeapon().pushBack(myWeapon);
}
