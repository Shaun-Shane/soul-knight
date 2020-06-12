#include "Knight.h"

#include "Attack/Weapon.h"
#include "Scene\BattleRoom.h"
#include "Scene\Hall.h"

Knight::Knight() : Entity(4, 5, 1.5f, .0f, .0f), armor(5), MP(200) {}

Knight::~Knight() {}

Animate* Knight::getAnimate() {
  //创建序列帧动画
  auto animation = Animation::create();

  //设置动画名字数组的长度
  char nameSize[30] = {0};

  //动画的循环4张图片
  for (int i = 1; i < 5; i++)

  {
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
  this->moveSpeedX = 0, this->moveSpeedY = 0;

  this->weapon = Weapon::create();
  this->weapon->setFireSpeed(24.0f);
  this->weapon->setAttack(1);
  this->weapon->bindSprite(Sprite::create("Weapon//pistol.png"),
                           LayerPlayer + 1);

  this->weapon->setPosition(Vec2(40, 20));

  this->weapon->setMPConsumption(0);
  this->addChild(weapon);

  isInvincible = false;

  registerKeyboardEvent();
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
        if (this->atBattleRoom != nullptr) {
          Weapon* weaponCheck = this->collisionWithWeaponCheck();
          Prop* prop = this->collisionWithCropCheck();
          if (weaponCheck != nullptr) {
            this->bindWeapon(weaponCheck);
            if (isRight == false) weapon->getSprite()->setFlippedX(true);
            break;
          } else if (prop != nullptr) {
            prop->useProps(this);
            prop->removeFromParent();
            this->atBattleRoom->getVecProps().eraseObject(prop);
            break;
          }
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
  if (this->MP >= 120) {
    log("using ultimate skill!");
    this->setMP(this->getMP() - 120);

    auto skillCircle = DrawNode::create();
    skillCircle->drawSolidCircle(Point(this->getContentSize().width / 2,
                                       this->getContentSize().height / 2),
                                 220.0f, CC_DEGREES_TO_RADIANS(360), 100,
                                 Color4F(1.0f, 0.8f, .0f, 0.3f));

    skillCircle->setGlobalZOrder(LayerPlayer);

    auto fadeIn = FadeIn::create(0.2f);
    auto fadeOut = FadeOut::create(0.3f);
    auto blink = Blink::create(0.5f, 2);

    auto sequence = Sequence::create(
        Spawn::create(Sequence::create(fadeIn, fadeOut, NULL), blink, NULL),
        RemoveSelf::create(), NULL);  //生成动作序列

    this->addChild(skillCircle);

    skillCircle->runAction(sequence);  //执行动画

    if (this->atBattleRoom == nullptr) return;

    Vector<Enemy*>& vecEnemy = atBattleRoom->getVecEnemy();

    for (auto& e : vecEnemy) {
      if (e == nullptr) continue;

      float enemyX = e->getPositionX(), enemyY = e->getPositionY();

      if (sqrt(pow(getPositionX() - enemyX, 2) +
               pow(getPositionY() - enemyY, 2)) <= 220.0f)
        e->retain(), e->removeFromParent();  //秒杀怪物 从父类移除
    }

    auto boss = atBattleRoom->getBoss();
    if (boss != nullptr && boss->getParent() != nullptr) {
        float bossX = boss->getPositionX(),
              bossY = boss->getPositionY();
        if (sqrt(pow(getPositionX() - bossX, 2) +
                 pow(getPositionY() - bossY, 2)) <= 220.0f)
          boss->retain(), boss->removeFromParent();  //秒杀boss 从父类移除
    }

    if (this->atBattleRoom != nullptr) {
      assert(atHall == nullptr);
      if (this->atBattleRoom->allKilled() == true) vecEnemy.clear();
    }
  }
}

void Knight::bindBattleRoom(BattleRoom* battleRoom) {
  atBattleRoom = battleRoom;
}

void Knight::bindHall(Hall* hall) { atHall = hall; }

void Knight::setNeedCreateBox(bool need) { this->needCreateBox = need; }

bool Knight::getNeedCreateBox() { return this->needCreateBox; }

INT32 Knight::getMP() { return this->MP; }

INT32 Knight::getArmor() { return this->armor; }

void Knight::setMP(INT32 mp) { this->MP = mp; }

void Knight::deductHP(INT32 delta) {
  preAttackedTime = curTime; //被攻击的时间 用于护甲的恢复判断

  armor -= delta; //护甲先直接减去扣血值
  if (armor < 0) { //小于零则HP加上护甲抵消后的扣血值
    HP = std::max(0, HP + armor);
  }
  armor = std::max(0, armor);
}

void Knight::resumeArmor() { //恢复护甲
  curTime++;
  if (armor == 5) return;
  //2秒未被攻击 则每隔一秒恢复一护甲
  if (curTime - preAttackedTime >= 120 && (curTime - preAttackedTime) % 55 == 0) {
    armor++;
  }
}

void Knight::weaponAttack(
    Vec2 last) {  //写得有点啰嗦，有空再精简，不过感觉不好精简了
  if (this->MP <= 0 && this->weapon->getMPConsumption() > 0) return;

  this->setMP(this->getMP() - this->weapon->getMPConsumption());

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
  (atBattleRoom != nullptr ? atBattleRoom : atHall)
      ->getVecPlayerBullet().pushBack(bullet);
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
  for (int i = 0; i < this->atBattleRoom->getVecProps().size(); ++i) {
    auto prop = this->atBattleRoom->getVecProps().at(i);
    Rect cropRect = prop->getBoundingBox();
    Rect kightRect = this->getBoundingBox();
    if (cropRect.intersectsRect(kightRect)) return prop;
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
