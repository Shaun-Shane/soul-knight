#include "Knight.h"

Knight::Knight() : Entity(4, 5, 1.5f, .0f, .0f), armor(5), MP(5) {}

Knight::~Knight() {}

bool Knight::init() {
  this->moveSpeedX = 0, this->moveSpeedY = 0;

  registerKeyboardEvent();

  this->scheduleUpdate(); 	
  return true; 
}

void Knight::registerKeyboardEvent() {
  auto listener = EventListenerKeyboard::create();

  listener->onKeyPressed = [&](EventKeyboard::KeyCode code, Event*) {
    switch (code) {
      case EventKeyboard::KeyCode::KEY_D:
        moveSpeedX = moveSpeed;
        getSprite()->setFlippedX(false);
        break;

      case EventKeyboard::KeyCode::KEY_W:
        moveSpeedY = moveSpeed;
        break;

      case EventKeyboard::KeyCode::KEY_A:
        moveSpeedX = -moveSpeed;
        getSprite()->setFlippedX(true);
        break;

      case EventKeyboard::KeyCode::KEY_S:
        moveSpeedY = -moveSpeed;
        break;
    }
  };

  listener->onKeyReleased = [&](EventKeyboard::KeyCode code, Event*) {
    moveSpeedX = .0f, moveSpeedY = .0f;
  };

  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

float Knight::getMoveSpeedX() { return moveSpeedX; }

float Knight::getMoveSpeedY() { return moveSpeedY; }