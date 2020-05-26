#include "EnemyController.h"
#include "Enemy.h"
#include "Actor\Knight.h"

bool EnemyController::init() {
  createEnemy();
  this->scheduleUpdate();
  return true;
}

void EnemyController::createEnemy() {
  Enemy* enemy = nullptr;
  Sprite* sprite = nullptr;

  srand(static_cast<unsigned int>(time(nullptr)));
  int maxI = static_cast<int>(CCRANDOM_0_1() * (MAX_ENEMY_NUM - MIN_ENEMY_NUM)) + MIN_ENEMY_NUM;
  
  for (int i = 0; i < maxI; i++) {
    enemy = Enemy::create();
    enemy->bindSprite(Sprite::create("Enemy//shooter.png"), 5);
    enemy->reset();

    this->addChild(enemy);
    vecEnemy.pushBack(enemy);
  }
}

void EnemyController::update(float delta) {
  Size visibleSize = Director::getInstance()->getVisibleSize();
  srand(static_cast<unsigned int>(time(nullptr)));
  for (auto enemy : vecEnemy) {
    if (enemy->isAlive()) {
      enemy->setPosition(visibleSize.width * CCRANDOM_0_1(), visibleSize.height * CCRANDOM_0_1());

      if (enemy->getHP() <= 0) {
        enemy->hide();
      } else {
        enemy->show();
      }

    }
  }
}

void EnemyController::bindKnight(Knight* knight) { this->knight = knight; }