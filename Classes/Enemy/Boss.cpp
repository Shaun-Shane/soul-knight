#include "Boss.h"
#include "Scene/BattleRoom.h"

Boss::Boss() {
	uniSkiTimeCount = 0;
}



Boss::~Boss() { }

bool Boss::init()
{
	this->setHP(500);
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
		if (!(uniSkiTimeCount % 400)) {
			srand(static_cast<unsigned>(time(nullptr)));
			aiChoice = rand() % 2;
		}//两种类型的动作，二选一
		
		if (aiChoice) {
			moveSpeedX = 0, moveSpeedY = 0;
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
				Point upLeftPos = battleRoom->getUpleftVertex();
				Point downRightPos = battleRoom->getDownRightVertex();
				for (unsigned i = 0; i < 4; i++) {
					if (myPos.x + 240 * DIRX[i] >= upLeftPos.x&& 
						myPos.x + 240 * DIRX[i] <= downRightPos.x&&
						myPos.y + 240 * DIRY[i] >= downRightPos.y &&
						myPos.y + 240 * DIRY[i] <= upLeftPos.y) {
						wayCanBeSelected.push_back(i);
					}//判断可走方向
				}

				srand(static_cast<unsigned>(time(nullptr)));
				wayOfPace = wayCanBeSelected[rand() % wayCanBeSelected.size()];
			}
			moveSpeedX = 3 * DIRX[wayOfPace], moveSpeedY = 3 * DIRY[wayOfPace];
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
	if (inRoom(battleRoom,Point(myPos.x + moveSpeedX, myPos.y + moveSpeedY))) {
		this->setPosition(myPos.x + moveSpeedX, myPos.y + moveSpeedY);
		spriteChangeDirection();
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
	 HP += 5 + (rand() % 8) * 2;
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
		moveSpeedX = knightPos.x - myPos.x;
		moveSpeedY = knightPos.y - myPos.y;
	}
	else {
		moveSpeedX = (knightPos.x - myPos.x) * (MAXFLASHRANGE / distance);
		moveSpeedY = (knightPos.y - myPos.y) * (MAXFLASHRANGE / distance);
	}
}

