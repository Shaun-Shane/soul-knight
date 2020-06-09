#include "Boss.h"

Boss::Boss() {
	attack = 5;
	HP = 50;
	uniSkiTimeCount = 0;
	
	this->bindSprite(Sprite::create("Enemy//boss.png"),5);
}

Boss::Boss(INT32 att, INT32 hp) {
	attack = att;
	HP = hp;
	uniSkiTimeCount = 0;

	this->bindSprite(Sprite::create("Enemy//boss.png"), 5);
}

Boss::~Boss() { }

/*操纵boss行为的函数，会根据帧数计时计算自己是需要普通动作还是调用大招*/
void Boss::aiOfBoss(Knight* knight, const std::vector<float>& boundaryOfRoom) {
	if (knight == nullptr) {
		return;
	}
	if (uniSkiTimeCount < UNISKITIMEINTERVAL) {
		//开始写没发大招的时候的动作
		if (!(uniSkiTimeCount % 400)) {
			srand(static_cast<unsigned>(time(nullptr)));
			aiChoice = rand() % 2;
		}//两种类型的动作，二选一
		const Point myPos = this->getPosition();
		if (aiChoice) {
			this->setPosition(myPos.x - knight->getMoveSpeedX(), myPos.y - knight->getMoveSpeedY());
			this->makeCoinside();
			if (!(uniSkiTimeCount%50)&&myPos.getDistance(knight->getPosition()) <= CLOSECOMBATRANGE) {
				knight->deductHP(2);
			}//近战
			else {
				/*后期在此处加上武器*/
			}
		}//静止
		else {
			if (!(uniSkiTimeCount % 80)) {
				wayCanBeSelected.clear();

				for (unsigned i = 0; i < 4; i++) {
					if (myPos.x + 240 * DIRX[i] >= boundaryOfRoom[LEFT] && 
						myPos.x + 240 * DIRX[i] <= boundaryOfRoom[RIGHT] &&
						myPos.y + 240 * DIRY[i] >= boundaryOfRoom[DOWN] &&
						myPos.y + 240 * DIRY[i] <= boundaryOfRoom[UP]) {
						wayCanBeSelected.push_back(i);
					}//判断可走方向
				}

				srand(static_cast<unsigned>(time(nullptr)));
				wayOfPace = wayCanBeSelected[rand() % wayCanBeSelected.size()];
			}
			this->setPosition(myPos.x + 3 * DIRX[wayOfPace] - knight->getMoveSpeedX(), 
				myPos.y + 3 * DIRY[wayOfPace] - knight->getMoveSpeedY());
			this->makeCoinside();
			if (!(uniSkiTimeCount % 50) &&
				myPos.getDistance(knight->getPosition()) <= CLOSECOMBATRANGE) {
				knight->deductHP(2);
			}//近战
			else {
				/*后期在此处加上武器*/
			}
		}
		uniSkiTimeCount++;
	}
	else {
		uniSkiTimeCount = 0;
		uniqueSkill(knight);
	}
}

void Boss::uniqueSkill(Knight* knight){
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
		flashMove(knight);
		break;
	default:
		break;
	}
}

void Boss::addHP(){
	srand(static_cast<unsigned>(time(nullptr)));
	 HP += 1 + (rand() % 4) * 2;
}

void Boss::heavilyAttackTheKnight(Knight* knight){
	const Point myPos = this->getPosition();
	const Point knightPos = knight->getPosition();
	const INT32 distance = myPos.getDistance(knightPos);
	if (distance <= HEAVYATTACKRANGE) {
		srand(static_cast<unsigned>(time(nullptr)));
		INT32 harmToKnight = 3 + rand() % 5;
		knight->deductHP(harmToKnight);
	}
}

void Boss::flashMove(Knight* knight){
	const Point myPos = this->getPosition();
	const Point knightPos = knight->getPosition();
	const INT32 distance = myPos.getDistance(knightPos);
	if (distance <= MAXFLASHRANGE) {
		this->setPosition(knightPos);
		this->makeCoinside();
	}
	else {
		float xMove = (knightPos.x - myPos.x) * (MAXFLASHRANGE / distance);
		float yMove = (knightPos.y - myPos.y) * (MAXFLASHRANGE / distance);
		this->setPosition(Point(myPos.x + xMove, myPos.y + yMove));
		this->makeCoinside();
	}
}

void Boss::makeCoinside(){
	this->getSprite()->setPosition(this->getPosition());
}

