#include "Knight.h"
#include "Scene\Hall.h"
#include "Scene\BattleRoom.h"
#include "Attack\Weapon.h"

Knight::Knight() : Entity(4, 5, 1.5f, .0f, .0f), armor(5), MP(5) {}

Knight::~Knight() {}

bool Knight::init() {
  this->moveSpeedX = 0, this->moveSpeedY = 0;
  this->weapon = Weapon::create();
  this->weapon->setAttack(4);
  this->weapon->setSpeed(0.5f);
  this->weapon->bindSprite(Sprite::create("weapon.png"));
  
  isInvincible = false, haveUltimateSkill = true;
  
  registerKeyboardEvent();

  this->scheduleUpdate(); 	
  return true; 
}

void Knight::registerKeyboardEvent() {
  auto listener = EventListenerKeyboard::create();
  listener->onKeyPressed = [&](EventKeyboard::KeyCode code, Event*) {
      static EventKeyboard::KeyCode last;

      switch (code) {
      case EventKeyboard::KeyCode::KEY_D:
        last = EventKeyboard::KeyCode::KEY_D;
        moveSpeedX = moveSpeed;
        getSprite()->setFlippedX(false);
        break;

      case EventKeyboard::KeyCode::KEY_W:
        last = EventKeyboard::KeyCode::KEY_W;
        moveSpeedY = moveSpeed;
        break;

      case EventKeyboard::KeyCode::KEY_A:
        last = EventKeyboard::KeyCode::KEY_A;
        moveSpeedX = -moveSpeed;
        getSprite()->setFlippedX(true);
        break;

      case EventKeyboard::KeyCode::KEY_S:
        last = EventKeyboard::KeyCode::KEY_S;
        moveSpeedY = -moveSpeed;
        break;

      case EventKeyboard::KeyCode::KEY_J:
        weaponAttack(last);
        break;

      case EventKeyboard::KeyCode::KEY_K:
        useUltimateSkill();
        break;
    }

    if (abs(moveSpeedX) > 0 && abs(moveSpeedY) > 0) //确保任意方向速度相同
      moveSpeedX /= sqrt(2.0f), moveSpeedY /= sqrt(2.0f);  
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

BattleRoom* Knight::getAtBattleRoom() { return atBattleRoom; }

Hall* Knight::getAtHall() { return atHall; }


void Knight::weaponAttack(EventKeyboard::KeyCode last) {
    Vec2 curPos = this->getPosition();
    Vec2 speed;
    Bullet* bullet;
    switch (last) {
    case EventKeyboard::KeyCode::KEY_D:
        weapon->getSpeed();
        speed.set(weapon->getSpeed(), 0);
        CCLOG("attack (1,0)");
        bullet= weapon->createBullet(speed, curPos, atBattleRoom, atHall);
        break;

    case EventKeyboard::KeyCode::KEY_W:
        
        speed.set(0, weapon->getSpeed());
        CCLOG("attack (0,1)");
        bullet = weapon->createBullet(speed, curPos, atBattleRoom, atHall);
        break;

    case EventKeyboard::KeyCode::KEY_A:
        
        speed.set(-(weapon->getSpeed()), 0);
        CCLOG("attack(-1,0)");
      bullet = weapon->createBullet(speed, curPos, atBattleRoom, atHall);
        break;

    case EventKeyboard::KeyCode::KEY_S:
        
        speed.set(0, -(weapon->getSpeed()));
        CCLOG("attack(0,-1)");
      bullet = weapon->createBullet(speed, curPos, atBattleRoom, atHall);
        break;
    }
  
}
