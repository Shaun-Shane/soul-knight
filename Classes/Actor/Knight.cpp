#include "Knight.h"
#include "Scene\Hall.h"
#include "Scene\BattleRoom.h"
#include "Attack/Weapon.h"

Knight::Knight() : Entity(4, 5, 1.5f, .0f, .0f), armor(5), MP(5) {}

Knight::~Knight() {}

bool Knight::init() {
  this->moveSpeedX = 0, this->moveSpeedY = 0;
  this->weapon=Weapon::create();
  this->weapon->setFireSpeed(2.0);
  this->weapon->setAttack(1);
  this->weapon->bindSprite(Sprite::create("Weapon//pistol.png"), LayerPlayer + 1);
  this->weapon->setScale(3);
  this->weapon->setPosition(Vec2(20, -40));
  this->addChild(weapon);

  
  isInvincible = false, haveUltimateSkill = true;
  
  registerKeyboardEvent();
  this->scheduleUpdate(); 	
  return true; 
}

void Knight::registerKeyboardEvent() {
  auto listener = EventListenerKeyboard::create();

  listener->onKeyPressed = [&](EventKeyboard::KeyCode code, Event*) {
    static Vec2 last;
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
      moveSpeedX = moveSpeed;
      getSprite()->setFlippedX(false);
      weapon->getSprite()->setFlippedX(false);
      break;

    case EventKeyboard::KeyCode::KEY_W:
      last.set(0, 1.0);
      moveSpeedY = moveSpeed;
      break;

    case EventKeyboard::KeyCode::KEY_A:
 
      last.set(-1.0, 0);
      moveSpeedX = -moveSpeed;
      getSprite()->setFlippedX(true);
      weapon->getSprite()->setFlippedX(true);
      break;

    case EventKeyboard::KeyCode::KEY_S:
      last.set(0, -1.0);
      moveSpeedY = -moveSpeed;
      break;

    case EventKeyboard::KeyCode::KEY_J:
      weaponAttack(last);
      break;

    case EventKeyboard::KeyCode::KEY_K:
      useUltimateSkill();
      break;
    }
  };

  listener->onKeyReleased = [&](EventKeyboard::KeyCode code, Event*) {
    switch (code) {
      case EventKeyboard::KeyCode::KEY_D:
        moveSpeedX = .0f;
        break;

      case EventKeyboard::KeyCode::KEY_W:
        moveSpeedY = .0f;
        break;

      case EventKeyboard::KeyCode::KEY_A:
        moveSpeedX = .0f;
        break;

      case EventKeyboard::KeyCode::KEY_S:
        moveSpeedY = .0f;
        break;
    }
  };

  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Knight::useUltimateSkill() {
  if (haveUltimateSkill) {
    log("using ultimate skill!");

    auto skillCircle = DrawNode::create();
    skillCircle->drawSolidCircle(getPosition(), 220.0f,
                                 CC_DEGREES_TO_RADIANS(360), 100,
                                 Color4F(1.0f, 0.8f, .0f, 0.3f));
    skillCircle->setGlobalZOrder(LayerPlayer);

    auto fadeIn = FadeIn::create(0.2f); 
    auto fadeOut = FadeOut::create(0.3f);
    auto blink = Blink::create(0.5f, 2);

    auto sequence = Sequence::create(
        Spawn::create(Sequence::create(fadeIn, fadeOut, NULL), blink, NULL),
        RemoveSelf::create(), NULL);

    if (this->atBattleRoom == nullptr) {
      assert(atHall != nullptr);
      atHall->addChild(skillCircle);

      skillCircle->runAction(sequence);
      return;
    }

    atBattleRoom->addChild(skillCircle);

    skillCircle->runAction(sequence);

    Vector<Enemy*>& vecEnemy = atBattleRoom->getVecEnemy();

    for (auto& e : vecEnemy) {
      if (e == nullptr) continue;

      float enemyX = e->getPositionX(), enemyY = e->getPositionY();

      if (sqrt(pow(getPositionX() - enemyX, 2) +
               pow(getPositionY() - enemyY, 2)) <= 220.0f)
        e->removeFromParent(); //秒杀怪物 从父类移除
    }

    bool allKilled = true; //判断是否全被击杀
    for (auto e : vecEnemy) {
      if (e->getParent() != nullptr) allKilled = false;
    }

    if (allKilled) vecEnemy.clear();

  }
}

void Knight::bindBattleRoom(BattleRoom* battleRoom) {
  atBattleRoom = battleRoom;
}

void Knight::bindHall(Hall* hall) { atHall = hall; }

float Knight::getMoveSpeedX() { return moveSpeedX; }

float Knight::getMoveSpeedY() { return moveSpeedY; }

void Knight::weaponAttack(Vec2 last) {          //写得有点啰嗦，有空再精简，不过感觉不好精简了
  Vec2 fireSpeed = last * (this->weapon->getFireSpeed());
  INT32 firePower = this->weapon->getAttack();
  Vec2 curPos = this->getPosition();
  Vec2 target;
  if (this->atBattleRoom != nullptr) {
    Vector<Enemy*>& vecEnemy = atBattleRoom->getVecEnemy();
    Enemy* nearNeast = nullptr;
    float distance = 99999;
    for (auto e : vecEnemy) {
      if (e->getParent() != nullptr) {
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
  Bullet* bullet = this->weapon->createBullet(fireSpeed, firePower);
  bullet->setPosition(curPos);
  (atBattleRoom != nullptr ? atBattleRoom : atHall)->addChild(bullet);
  (atBattleRoom != nullptr ? atBattleRoom : atHall)->getVecPlayerBullet().pushBack(bullet);
}


 
 

  


      

  