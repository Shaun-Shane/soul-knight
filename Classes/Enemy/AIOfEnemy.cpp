#include "AIOfEnemy.h"

/*
bindEnemy是用来捆绑敌人的 既然要合并到Enemy类里头应该就可以删掉了
与之匹配的myEnemy变量也是
aiOfEnemy函数就是控制敌人走步的，
根据敌人与主角的距离判断是调用patrolRoute还是attackTheKnight
patrolRoute是巡逻路线，
先通过计算怪物到各个边界的距离来判断哪些方向是可以走的存入wayCanBeSelected，
然后随机选择其中一个方向存入wayOfPace，然后之后的40步，如果没有受到主角接近的干扰，
就按照wayOfPace存入的步数走，用paceCount记录这一轮走了多少步，
就是每走一步paceCount自增一
attackTheKnight函数的话也会计算敌人跟骑士的距离，如果近到了一定就直接按照近战攻击扣血，
否则就使用武器【因为武器的类还没出来所以那一块就暂时空着了】
*/

AIOfEnemy::~AIOfEnemy() {}

void AIOfEnemy::bindEnemy(Enemy* enemy) { myEnemy = enemy; }

AIOfEnemy::AIOfEnemy() {
	myEnemy = nullptr;
	paceCount = 0;
	wayOfPace = -1;  //-1代表未确定方向
	attackTimeCount = 1;
}

void AIOfEnemy::patrolRoute(const BattleRoom* battleRoom, Knight* knight,
	const std::vector<float>& boundaryOfRoom) {
	const Point enemyPos = myEnemy->getPosition();

	if (paceCount % 40) {
		myEnemy->setPosition(
			Point(enemyPos.x + DIRX[wayOfPace] - knight->getMoveSpeedX(),
				enemyPos.y + DIRY[wayOfPace] - knight->getMoveSpeedY()));
		paceCount++;
		return;
	}

	paceCount = 1;  //设置为1，避免下一轮被paceCount%40识别为false
	wayCanBeSelected.clear();
	for (INT32 i = 0; i <= 3; i++) {
		if (enemyPos.x + DIRX[i] * 40 < boundaryOfRoom[RIGHT] &&
			enemyPos.x + DIRX[i] * 40 > boundaryOfRoom[LEFT] &&
			enemyPos.y + DIRY[i] * 40 <= boundaryOfRoom[UP] &&
			enemyPos.y + DIRY[i] * 40 >= boundaryOfRoom[DOWN]) {
			wayCanBeSelected.push_back(i);
		}
	}  //选取可以走的方向

	srand(static_cast<unsigned int>(time(nullptr)));
	wayOfPace = wayCanBeSelected[rand() % wayCanBeSelected.size()];
	myEnemy->setPosition(
		Point(enemyPos.x + DIRX[wayOfPace] - knight->getMoveSpeedX(),
			enemyPos.y + DIRY[wayOfPace] - knight->getMoveSpeedY()));

}  //在没探测到骑士的时候正常的巡逻路线

void AIOfEnemy::aiOfEnemy(Knight* knight, BattleRoom* battleRoom,
	const std::vector<float>& boundaryOfRoom) {
	if (knight == nullptr || battleRoom == nullptr) {
		return;
	}

	const Point enemyPos = myEnemy->getPosition();
	const Point knightPos = knight->getPosition();
	const INT32 disBetweenEnemyAndKnight =
		enemyPos.getDistance(knightPos);  //获取二者距离，用于后续判断

	if (disBetweenEnemyAndKnight > SIGHTRANGE) {
		patrolRoute(battleRoom, knight, boundaryOfRoom);
	}
	else {
		paceCount = 0;
		wayOfPace = -1;
		if (disBetweenEnemyAndKnight > ATTACKRANGE) {
			myEnemy->setPosition(
				Point(enemyPos.x +
					2 * (knightPos.x - enemyPos.x) / disBetweenEnemyAndKnight -
					knight->getMoveSpeedX(),
					enemyPos.y +
					2 * (knightPos.y - enemyPos.y) / disBetweenEnemyAndKnight -
					knight->getMoveSpeedY()));
		}
		else {
			attackTheKnight(knight, disBetweenEnemyAndKnight);
		}
	}
}

void AIOfEnemy::attackTheKnight(Knight* knight,
	INT32 disBetweenEnemyAndKnight) {
	if (disBetweenEnemyAndKnight <= 5) {
		if (attackTimeCount % 40 == 0) {
			knight->deductHP(3);
			attackTimeCount = 1;
		}
		attackTimeCount++;
		return;
	}

	else {
		const Point enemyPos = myEnemy->getPosition();
		const Point knightPos = knight->getPosition();
		myEnemy->setPosition(Point(
			enemyPos.x + 2 * (knightPos.x - enemyPos.x) / disBetweenEnemyAndKnight -
			knight->getMoveSpeedX(),
			enemyPos.y + 2 * (knightPos.y - enemyPos.y) / disBetweenEnemyAndKnight -
			knight->getMoveSpeedY()));
	}  //等武器那一块出来加上武器
}
