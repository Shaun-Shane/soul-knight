#include "Statue.h"

bool Statue::init() { 
  playerVisited = false;

  srand(static_cast<unsigned int>(time(nullptr)));
  this->statueType = rand() % CNT_TYPE;

  switch (this->statueType) {
    case RESUME_HP:
      break;
    case ADD_HP:
      break;
    case DOUBLE_HP:
      break;
    case ADD_ARMOR:
      break;
    case DOUBLE_ARMOR:
      break;
    case DOUBLE_DAMAGE:
      break;
    case ADD_MOVE_SPEED:
      break;
  }
  // this->label->setString();
  return true; 
}

void Statue::bindSprite(Sprite* sprite, INT32 layer) { 
  //将Node中心和sprite中心重合
  Size size = sprite->getContentSize();
  this->setContentSize(size);  //设置大小
  this->setAnchorPoint(Point(0.5f, 0.5f));  //设置锚点
  setPosition(Point(.0f, .0f));

  //分割精灵
  auto tmpSprite = Sprite::create(sprite->getResourceName(), Rect(0, 0, 120, 100));
  tmpSprite->setPosition(Point(60, 150));
  tmpSprite->setGlobalZOrder(layer);
  this->addChild(tmpSprite);

  tmpSprite = Sprite::create(sprite->getResourceName(), Rect(0, 100, 120, 100));
  tmpSprite->setPosition(Point(60, 50));
  tmpSprite->setGlobalZOrder(LayerPlayer - 1);
  this->addChild(tmpSprite);
}

Label* Statue::getLabel() const { return this->label; }

void Statue::setGoldCost(INT32 cost) { this->goldCost = cost; }

INT32 Statue::getGoldCost() const { return this->goldCost; }

INT32 Statue::getStatueType() const { return this->statueType; }
