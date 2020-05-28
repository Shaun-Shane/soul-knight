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
        moveSpeedX = moveSpeed, moveSpeedY = .0f;
        getSprite()->setFlippedX(false);
        break;

      case EventKeyboard::KeyCode::KEY_W:
        moveSpeedX = .0f, moveSpeedY = moveSpeed;
        break;

      case EventKeyboard::KeyCode::KEY_A:
        moveSpeedX = -moveSpeed, moveSpeedY = .0f;
        getSprite()->setFlippedX(true);
        break;

      case EventKeyboard::KeyCode::KEY_S:
        moveSpeedX = .0f, moveSpeedY = -moveSpeed;
        break;
    }
  };

  listener->onKeyReleased = [&](EventKeyboard::KeyCode code, Event*) {
    moveSpeedX = .0f, moveSpeedY = .0f;
  };

  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

