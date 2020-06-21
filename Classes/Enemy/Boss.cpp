#include "Boss.h"
#include "Actor/FlowWord.h"
#include "Scene/BattleRoom.h"

Boss::Boss() {
	uniSkiTimeCount = 0;
}

void Boss::spriteChangeDirection() {
	if (moveSpeedX == 0) {
		return;
	}
	if (moveSpeedX < 0) {
		this->getSprite()->setFlipX(true);
		mySword->setPosition(60.0, 125.0);
	}
	else {
		this->getSprite()->setFlipX(false);
		mySword->setPosition(140.0, 125.0);
	}
}

Boss::~Boss() { }

bool Boss::init()
{
    HP = maxHP = 300;
	lastHP = HP;
	attack = 6;
	return true;
}

/*操纵boss行为的函数，会根据帧数计时计算自己是需要普通动作还是调用大招*/
void Boss::aiOfBoss(Knight* knight,BattleRoom* battleRoom) {
	if (knight == nullptr) {
		return;
	}
	const Point myPos = this->getPosition();
	if (uniSkiTimeCount < UNISKITIMEINTERVAL) {
		//开始写没发大招的时候的动作
		

		if (!(uniSkiTimeCount % 60)) {
			wayCanBeSelected.clear();
			Point upLeftPos = battleRoom->getUpleftVertex();
			Point downRightPos = battleRoom->getDownRightVertex();
			for (unsigned i = 0; i < 4; i++) {
				if (myPos.x + 210 * DIRX[i] >= upLeftPos.x &&
					myPos.x + 210 * DIRX[i] <= downRightPos.x &&
					myPos.y + 210 * DIRY[i] >= downRightPos.y &&
					myPos.y + 210 * DIRY[i] <= upLeftPos.y) {
					wayCanBeSelected.push_back(i);
				}//判断可走方向
			}

			srand(static_cast<unsigned>(time(nullptr)));
			wayOfPace = wayCanBeSelected[rand() % wayCanBeSelected.size()];
		}
		moveSpeedX = 3.5 * DIRX[wayOfPace], moveSpeedY = 3.5 * DIRY[wayOfPace];
		if (!(uniSkiTimeCount % 20) &&
			myPos.getDistance(knight->getPosition()) <= CLOSECOMBATRANGE) {
			knight->deductHP(4);
		}//近战
		else {
			/*后期在此处加上武器*/
		}

		uniSkiTimeCount++;
	}
	else {
		uniSkiTimeCount = 0;
		uniqueSkill(knight,battleRoom);
	}
	if (lastHP != HP) {
		if (lastHP > HP) {
			beAttacked = true;
		}
		lastHP = HP;
	}
	if (inRoom(battleRoom,Point(myPos.x + moveSpeedX, myPos.y + moveSpeedY))) {
		this->setPosition(myPos.x + moveSpeedX, myPos.y + moveSpeedY);
		spriteChangeDirection();
	}
	if (beAttacked) {
		shake(battleRoom);
	}
}

void Boss::createSword(Sprite* swordSprite){
	mySword = swordSprite;
	swordSprite->setPosition(140.0, 125.0);
	swordSprite->setGlobalZOrder(LayerPlayer);
	swordSprite->setAnchorPoint(Point(0.5, 1));//方便挥剑
	this->addChild(swordSprite);

}

void Boss::uniqueSkill(Knight* knight, BattleRoom* battleRoom){
	srand(static_cast<unsigned>(time(nullptr)));
	int choice = rand() % 3;
	switch (choice)
	{
	case 0:
		addHP();
		break;
	case 1:
		heavilyAttackTheKnight(knight);
		break;
	case 2:
		flashMove(knight,battleRoom);
		break;
	default:
		break;
	}
}

void Boss::addHP(){
	srand(static_cast<unsigned>(time(nullptr)));
	INT32 hpPlus =40 + (rand() % 30) * 2;
	/*判断是否超过上限*/
	if (HP + hpPlus <= 500) {
		HP += hpPlus;
	}
	else {
		hpPlus = 500 - HP;
		HP = 500;
	}

	/*加血特效*/
	 FlowWord* flowWord = FlowWord::create();
	 this->addChild(flowWord);
	 flowWord->showWord(+hpPlus,
		 getSprite()->getPosition() +
		 Vec2(0, this->getContentSize().height / 2.2f));
}

void Boss::heavilyAttackTheKnight(Knight* knight){
	const Point myPos = this->getPosition();
	const Point knightPos = knight->getPosition();
	const INT32 distance = myPos.getDistance(knightPos);

	if (distance <= HEAVYATTACKRANGE) {
		srand(static_cast<unsigned>(time(nullptr)));
		INT32 harmToKnight = 4 + rand() % 4;
		knight->deductHP(harmToKnight);
	}//在距离内就扣血

	/*扣血范围圈*/
	auto skillCircle = DrawNode::create();
	skillCircle->drawSolidCircle(this->getSprite()->getPosition(), HEAVYATTACKRANGE,
		CC_DEGREES_TO_RADIANS(360), 100, Color4F(1.0f, 0.2f, 0.2f, 0.2f));
	skillCircle->setGlobalZOrder(LayerPlayer - 1);

	auto fadeIn = FadeIn::create(0.2f);
	auto fadeOut = FadeOut::create(0.3f);
	auto blink = Blink::create(0.5f, 2);

	auto sequence = Sequence::create(
		Spawn::create(Sequence::create(fadeIn, fadeOut, NULL), blink, NULL),
		RemoveSelf::create(), NULL);  //生成动作序列

	this->addChild(skillCircle);

	skillCircle->runAction(sequence);  //执行动画

	/*挥剑动作*/
	if (this->getSprite()->isFlipX()) {
		auto rotateBy = RotateBy::create(0.5f, 150.0f);
		auto rotateBy2 = RotateBy::create(0.1f, -150.0f);
		auto seq = Sequence::create(rotateBy, rotateBy2, nullptr);
		mySword->runAction(seq);
	}
	else {
		auto rotateBy = RotateBy::create(0.5f, -150.0f);
		auto rotateBy2 = RotateBy::create(0.1f, 150.0f);
		auto seq = Sequence::create(rotateBy, rotateBy2, nullptr);
		mySword->runAction(seq);
	}
}

void Boss::flashMove(Knight* knight, BattleRoom* battleRoom) {
	const Point myPos = this->getPosition();
	const Point knightPos = knight->getPosition();
	const float distance = myPos.getDistance(knightPos);
	if (distance <= MAXFLASHRANGE) {
		moveSpeedX = knightPos.x - myPos.x;
		moveSpeedY = knightPos.y - myPos.y;
	}//在最大瞬移距离内就直接瞬移过去
	else {
		moveSpeedX = (knightPos.x - myPos.x) * (static_cast<float>(MAXFLASHRANGE) / distance);
		moveSpeedY = (knightPos.y - myPos.y) * (static_cast<float>(MAXFLASHRANGE) / distance);
		log("%d,%d", moveSpeedX, moveSpeedY);
	}//在最大瞬移距离外就往该方向瞬移最大距离
	if (distance <= 10) {
		srand(static_cast<unsigned>(time(nullptr)));
		knight->deductHP(rand() % 6);
	}
	auto fadeIn = FadeIn::create(0.02f);
	auto fadeOut = FadeOut::create(0.03f);

	auto tempSprite = Sprite::create("Enemy//boss.png");
	tempSprite->setGlobalZOrder(LayerPlayer - 1);
	tempSprite->setPosition(myPos.x+moveSpeedX / 2, myPos.y+moveSpeedY / 2);
	auto sequence = Sequence::create(Spawn::create(Sequence::create(fadeIn, fadeOut, NULL),NULL), 
		RemoveSelf::create(), NULL);

	battleRoom->addChild(tempSprite);
	tempSprite->runAction(sequence);
}

