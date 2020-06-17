#include "Enemy.h"
#include "Scene/BattleRoom.h"

Enemy::Enemy() { enemyIsAlive = false; }

Enemy::~Enemy() {}

bool Enemy::init() { 
  HP = 5;
  lastHP = HP;
  isKilled = false;
  return true; 
}

void Enemy::show() {
  if (getSprite() != nullptr) {
    setVisible(true); //可见
    enemyIsAlive = true;
  } 
}

void Enemy::hide() {
  if (getSprite() != nullptr) {
    setVisible(false); //不可见
    reset(); //重置敌人数据
    enemyIsAlive = false;
  }
}

void Enemy::reset() {
  if (getSprite() != nullptr) {
    // randomly reset positon
    // need update
    setPosition(Point(800 + CCRANDOM_0_1() * 2000, 200 - CCRANDOM_0_1() * 100));
  }
}

bool Enemy::isAlive() { return enemyIsAlive;}

//碰撞检查 可能没用
bool Enemy::isCollideWithKnight(Knight * knight) {
  //获取碰撞对象的bounding box
  Rect entityRec = knight->getBoundingBox();

  Point enemyPos = getPosition();

  //判断bounding box 和 怪物中心点是否有交集
  return entityRec.containsPoint(enemyPos);
}

void Enemy::setType(int type){
    enemyType = type;
    boarRushCount = 0;
    setAttackRange();
}

void Enemy::setAttackRange(){
    switch (enemyType)
    {
    case 0:
        ATTACKRANGE = 200;//弓箭手
        break;
    case 1:
        ATTACKRANGE = SIGHTRANGE;//野猪
        break;
    case 2:
        ATTACKRANGE = 50;//持矛怪物
        break;
    default://暂时还没想出别的
        break;
    }
}

bool Enemy::inRoom(const BattleRoom* battleRoom,Point myPos){
	Point upLeftPos = battleRoom->getUpleftVertex();
	Point downRightPos = battleRoom->getDownRightVertex();
    if (myPos.x>=upLeftPos.x&&myPos.x<=downRightPos.x&&
        myPos.y>=downRightPos.y&&myPos.y<=upLeftPos.y) {
        return true;
    }
    return false;
}

void Enemy::spriteChangeDirection() {
    if (moveSpeedX == 0) {
        return;
    }
    if (moveSpeedX < 0) {
        this->getSprite()->setFlipX(true);
    }
    else {
        this->getSprite()->setFlipX(false);
    }
}

void Enemy::shake(const BattleRoom* battleRoom){
    auto enemyPos = this->getPosition();
    if (shakeTimeCount++ % 2) {
        if (inRoom(battleRoom, Point(enemyPos.x + 25, enemyPos.y))) {
            this->setPosition(Point(enemyPos.x + 25, enemyPos.y));
        }
    }
    else {
		if (inRoom(battleRoom, Point(enemyPos.x - 25, enemyPos.y))) {
			this->setPosition(Point(enemyPos.x - 25, enemyPos.y));
		}
    }
    if (shakeTimeCount >= 4) {
        shakeTimeCount = 0;
        beAttacked = false;
    }
}

void Enemy::patrolRoute(const BattleRoom* battleRoom, Knight* knight) {
  const Point enemyPos = this->getPosition();

  if (paceCount % 40) {
      moveSpeedX = 3 * DIRX[wayOfPace];
      moveSpeedY = 3 * DIRY[wayOfPace];
    //3.0f是合移动速度
    paceCount++;
    return;
  }

  paceCount = startCount;  //设置为startCount，避免下一轮被paceCount%40识别为false
  wayCanBeSelected.clear();

  for (INT32 dir = 0; dir < CNTDIR; dir++) { // 4个可以走的方向
    Point upLeftPos = battleRoom->getUpleftVertex();
    Point downRightPos = battleRoom->getDownRightVertex();

    if (upLeftPos.x  < enemyPos.x + DIRX[dir] * 120 && 
        enemyPos.x + DIRX[dir] * 120 < downRightPos.x &&
        downRightPos.y < enemyPos.y + DIRY[dir] * 120 &&
        enemyPos.y + DIRY[dir] * 120 < upLeftPos.y) {  //判断边界 40步=120.0f
      wayCanBeSelected.push_back(dir);
    }
  }  //选取可以走的方向

  srand(static_cast<unsigned int>(time(nullptr)));

  assert(wayCanBeSelected.size() != 0);

  wayOfPace = wayCanBeSelected[rand() % wayCanBeSelected.size()];
  moveSpeedX = 3 * DIRX[wayOfPace];
  moveSpeedY = 3 * DIRY[wayOfPace];

}  //在没探测到骑士的时候正常的巡逻路线

void Enemy::aiOfEnemy(Knight* knight, const BattleRoom* battleRoom) {
  if (knight == nullptr || battleRoom == nullptr) {
    return;
  }

  const Point enemyPos = this->getPosition();
  const Point knightPos = knight->getPosition();
  const float disBetweenEnemyAndKnight =
      enemyPos.getDistance(knightPos);  //获取二者距离，用于后续判断

  if (disBetweenEnemyAndKnight > SIGHTRANGE) {
    patrolRoute(battleRoom, knight);
    followCount = 0;
  } else {
    paceCount = 0;
    wayOfPace = -1;
    if (disBetweenEnemyAndKnight > ATTACKRANGE) {
        if (followCount >= 25) {
            followCount = 0;
        }
        if (followCount == 0) {
            srand(static_cast<unsigned>(time(nullptr)));
            shiftSeed = rand_0_1() - 0.5;
        }
        moveSpeedX = 3.0f * (knightPos.x - enemyPos.x) / disBetweenEnemyAndKnight + 3.0f * shiftSeed;
        moveSpeedY = 3.0f * (knightPos.y - enemyPos.y) / disBetweenEnemyAndKnight + 3.0f * shiftSeed;
      followCount++;
    } else {
      attackTheKnight(knight, disBetweenEnemyAndKnight,battleRoom);
    }
  }
  if (lastHP != HP) {
      lastHP = HP;
      beAttacked = true;
  }
  if (inRoom(battleRoom, Point(enemyPos.x + moveSpeedX, enemyPos.y + moveSpeedY))) {
      this->setPosition(enemyPos.x + moveSpeedX, enemyPos.y + moveSpeedY);
      spriteChangeDirection();
  }
  if (beAttacked) {
	  shake(battleRoom);
  }
}

void Enemy::attackTheKnight(Knight* knight,
    float disBetweenEnemyAndKnight,const BattleRoom* battleRoom) {
    if (disBetweenEnemyAndKnight < ATTACKRANGE) {
        switch (enemyType)
        {
        case 0:
            archerAttack(knight, disBetweenEnemyAndKnight);
            break;
        case 1:
            boarAttack(knight, disBetweenEnemyAndKnight,battleRoom);
            break;
        case 2:
            spearAttack(knight, disBetweenEnemyAndKnight);
        default:
            break;
        }
    }
}

/*弓箭手属于典型远攻类型，所以到达ATTACKRANGE之后不会再移动，而是直接射击*/
void Enemy::archerAttack(Knight* knight, float disBetweenEnemyAndKnight){
	auto enemyPos = this->getPosition();
	auto knightPos = knight->getPosition();

    if (attackTimeCount >= 80) {
        knight->deductHP(3);
        log("%d", knight->getHP());
        attackTimeCount = 0;
    }
    else {
        attackTimeCount++;
    }
}

/*野猪属于冲撞攻击类，所以在比较靠近骑士之后会开启加速冲撞攻击*/
void Enemy::boarAttack(Knight* knight, float disBetweenEnemyAndKnight, const BattleRoom* battleRoom) {
	auto enemyPos = this->getPosition();
    auto knightPos = knight->getPosition();

    if (haveAttacked) {
        if (attackTimeCount++ >= 60) {
            haveAttacked = false;
            attackTimeCount = 0;
            return;
        }
        if (inRoom(battleRoom,Point(enemyPos.x + 5.0f * boarBumpDirection[0] + 0.5f * shiftSeed,
            enemyPos.y + 5.0f * boarBumpDirection[1] + 0.5f * shiftSeed))) {
            moveSpeedX = 5.0f * boarBumpDirection[0] + 0.5f * shiftSeed;
            moveSpeedY = 5.0f * boarBumpDirection[1] + 0.5f * shiftSeed;
        }
        return;
    }

    if (!boarRest&&boarRushCount < 90) {
        if (disBetweenEnemyAndKnight >= 50) {
            moveSpeedX = 6.0f * (knightPos.x - enemyPos.x) / disBetweenEnemyAndKnight;
            moveSpeedY = 6.0f * (knightPos.y - enemyPos.y) / disBetweenEnemyAndKnight;
            boarBumpDirection[0] = (knightPos.x - enemyPos.x) / disBetweenEnemyAndKnight;
            boarBumpDirection[1] = (knightPos.y - enemyPos.y) / disBetweenEnemyAndKnight;//保存最近的方向，给之后冲走使用
        }
        else{
            moveSpeedX = 8.0f * boarBumpDirection[0];
            moveSpeedY = 8.0f * boarBumpDirection[1];
        }
        boarRushCount++;
    }
    else if (!boarRest&&boarRushCount >= 90) {
        boarRest = true;
    }
    else if (boarRushCount <= 0) {
        boarRest = false;
    }
    else {
        boarRushCount--;
    }

    if (attackTimeCount >= 80 && disBetweenEnemyAndKnight < 10) {
        boarBumpDirection[0] = (knightPos.x - enemyPos.x) / disBetweenEnemyAndKnight;
        boarBumpDirection[1] = (knightPos.y - enemyPos.y) / disBetweenEnemyAndKnight;//保存最近的方向，给之后冲走使用
        knight->deductHP(2);
        log("%d", knight->getHP());
        attackTimeCount = 0;
        haveAttacked = true;
    }
    else {
        attackTimeCount++;
    }  
}

void Enemy::spearAttack(Knight* knight, float disBetweenEnemyAndKnight) {
	auto enemyPos = this->getPosition();
	auto knightPos = knight->getPosition();
	if (!haveAttacked) {
		if (disBetweenEnemyAndKnight >= 15) {
			moveSpeedX = 4.0f * (knightPos.x - enemyPos.x) / disBetweenEnemyAndKnight;
			moveSpeedY = 4.0f * (knightPos.y - enemyPos.y) / disBetweenEnemyAndKnight;
		}
		else {
			moveSpeedX = 7.5f * (knightPos.x - enemyPos.x) / disBetweenEnemyAndKnight;
			moveSpeedY = 7.5f * (knightPos.y - enemyPos.y) / disBetweenEnemyAndKnight;
		}
		if (disBetweenEnemyAndKnight <= 5) {
			knight->deductHP(3);
			haveAttacked = true;
		}
	}
	else {
		if (restCount >= 120) {
			restCount = 0;
			haveAttacked = false;
		}
		restCount++;
	}
}

