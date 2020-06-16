#include "Entity.h"
#include"FlowWord.h"

Entity::Entity() : attack(0), HP(0), attackSpeed(.0f), moveSpeedX(.0f), moveSpeedY(.0f) { this->sprite = nullptr; }

Entity::Entity(INT32 _attack, INT32 _HP, float _attackSpeed, float _moveSpeedX, float _moveSpeedY)
    : attack(_attack), HP(_HP), attackSpeed(_attackSpeed), moveSpeedX(_moveSpeedX), moveSpeedY(_moveSpeedY) {
  this->sprite = nullptr;
}

Entity::Entity(Sprite* sprite) : attack(3), HP(5), attackSpeed(0.5f) {
  this->sprite = sprite;
  this->addChild(this->sprite);
}

Entity::~Entity() {}

Sprite* Entity::getSprite() { return this->sprite; }

void Entity::bindSprite(Sprite* sprite, INT32 layer) {
  this->sprite = sprite;
  this->sprite->setGlobalZOrder(layer);

  //将entity中心和sprite中心重合
  Size size = this->sprite->getContentSize();
  this->setContentSize(size); //设置大小

  this->setAnchorPoint(Point(0.5f, 0.5f)); //设置锚点
  setPosition(Point(.0f, .0f));


  this->addChild(sprite);
  sprite->setPosition(Point(size.width / 2, size.height / 2));
}

void Entity::addShadow(const Point& center, INT32 layer) { //添加阴影
  auto entityCircle = DrawNode::create();

  entityCircle->drawSolidCircle(center, this->getContentSize().width / 2.7f,
                                CC_DEGREES_TO_RADIANS(360), 20, 1.0f, 0.35f,
                                Color4F(.0f, .0f, .0f, 0.5f));

  entityCircle->setGlobalZOrder(layer - 1);
  this->addChild(entityCircle);
}

INT32 Entity::getHP() { //return HP of this entity
  if (getSprite() == nullptr) {
    log("null Sprite !");
    return 0;
  }
  return this->HP;
}

float Entity::getMoveSpeedX() { return moveSpeedX; }

float Entity::getMoveSpeedY() { return moveSpeedY; }

void Entity::deductHP(INT32 delta) { //minus HP of this entity
  if (getSprite() == nullptr) return;
  this->HP = std::max(0, this->HP - delta);

  /*受伤特效*/
  FlowWord* flowWord = FlowWord::create();
  this->addChild(flowWord);
  flowWord->showWord(-delta, getSprite()->getPosition());
}

void Entity::setHP(INT32 HP) { this->HP = HP; }
