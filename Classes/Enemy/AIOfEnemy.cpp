#include "AIOfEnemy.h"

AIOfEnemy::AIOfEnemy() {
	myEnemy = nullptr;
	paceCount = 0;
	wayOfPace = -1;//-1代表未确定方向
}

AIOfEnemy::~AIOfEnemy() {}

void AIOfEnemy::bindEnemy(Enemy* enemy) {
	myEnemy = enemy;
}

void AIOfEnemy::patrolRoute(const BattleRoom* battleRoom,Knight* knight, const std::vector<float>& boundaryOfRoom) {
	const Point enemyPos = myEnemy->getPosition();
	
	if (paceCount % 40) {
		myEnemy->setPosition(Point(enemyPos.x + DIRX[wayOfPace]-knight->getMoveSpeedX(), enemyPos.y + DIRY[wayOfPace]-knight->getMoveSpeedY()));
		myEnemy->makeCoinside();
		paceCount++;
		return;
	}

	paceCount = 1;//设置为1，避免下一轮被paceCount%40识别为false
	wayCanBeSelected.clear();
	for (INT32 i = 0; i <= 3; i++) {
		if (enemyPos.x + DIRX[i] * 40 < boundaryOfRoom[RIGHT] && enemyPos.x + DIRX[i] * 40 > boundaryOfRoom[LEFT]
			&& enemyPos.y + DIRY[i] * 40 <= boundaryOfRoom[UP]&& enemyPos.y + DIRY[i] * 40 >= boundaryOfRoom[DOWN]) {
			wayCanBeSelected.push_back(i);
		}
	}//选取可以走的方向

	srand(static_cast<unsigned int>(time(nullptr)));
	wayOfPace = wayCanBeSelected[rand()% wayCanBeSelected.size()];
	myEnemy->setPosition(Point(enemyPos.x + DIRX[wayOfPace] - knight->getMoveSpeedX(), enemyPos.y + DIRY[wayOfPace] - knight->getMoveSpeedY()));
	myEnemy->makeCoinside();

}//在没探测到骑士的时候正常的巡逻路线

void AIOfEnemy::aiOfEnemy(Knight* knight, BattleRoom* battleRoom, const std::vector<float>& boundaryOfRoom) {
	if (knight == nullptr || battleRoom == nullptr) {
		return;
	}

	const Point enemyPos = myEnemy->getPosition();
	const Point knightPos = knight->getPosition();
	const INT32 disBetweenEnemyAndKnight = enemyPos.getDistance(knightPos);//获取二者距离，用于后续判断

	if (disBetweenEnemyAndKnight > SIGHTRANGE) {
		patrolRoute(battleRoom,knight,boundaryOfRoom);
	}
	else {
		paceCount = 0;
		wayOfPace = -1;
		if (disBetweenEnemyAndKnight > ATTACKRANGE) {
			myEnemy->setPosition(Point(enemyPos.x + 2 * (knightPos.x - enemyPos.x) / disBetweenEnemyAndKnight-knight->getMoveSpeedX(), 
				enemyPos.y + 2 * (knightPos.y - enemyPos.y) / disBetweenEnemyAndKnight-knight->getMoveSpeedY()));
			myEnemy->makeCoinside();
		}
		else {
			attackTheKnight(knight,disBetweenEnemyAndKnight);
		}
	}
}

void AIOfEnemy::attackTheKnight(Knight* knight, INT32 disBetweenEnemyAndKnight) {
	if (disBetweenEnemyAndKnight <= 5) {
		knight->deductHP(3);
		return;
	}

	else {
		const Point enemyPos = myEnemy->getPosition();
		const Point knightPos = knight->getPosition();
		myEnemy->setPosition(Point(enemyPos.x + 2 * (knightPos.x - enemyPos.x) / disBetweenEnemyAndKnight - knight->getMoveSpeedX(),
			enemyPos.y + 2 * (knightPos.y - enemyPos.y) / disBetweenEnemyAndKnight - knight->getMoveSpeedY()));
		myEnemy->makeCoinside();
	}//等武器那一块出来加上武器
}

