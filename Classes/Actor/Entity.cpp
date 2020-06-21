#include "Entity.h"
#include"FlowWord.h"

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

void Entity::showDeathEffect() {
	auto blink = Blink::create(0.3f, 3);
	auto FadeOut = FadeOut::create(0.3f);

	auto callFunc = CallFunc::create([&]() {
		this->retain();
		this->removeFromParent();
		});

	auto sequence = Sequence::create(Spawn::create(blink, FadeOut, NULL), callFunc, NULL);
	this->getSprite()->runAction(sequence);
}

INT32 Entity::getHP() const { //return HP of this entity
	return this->HP;
}

float Entity::getMoveSpeedX() const { return moveSpeedX; }

void Entity::setMoveSpeedX(float speedX) { this->moveSpeedX = speedX; }

float Entity::getMoveSpeedY() const { return moveSpeedY; }

void Entity::setMoveSpeedY(float speedY) { this->moveSpeedY = speedY; }

void Entity::deductHP(INT32 delta) { //minus HP of this entity
	if (getSprite() == nullptr) return;
	this->HP = std::max(0, this->HP - delta);

	/*受伤特效*/
	FlowWord* flowWord = FlowWord::create();
	this->addChild(flowWord);
	flowWord->showWord(-delta,
		getSprite()->getPosition() +
		Vec2(0, this->getContentSize().height / 2.2f));
}

void Entity::setHP(INT32 HP) { this->HP = std::min(HP, maxHP); }

INT32 Entity::getMaxHP() const { return this->maxHP; }

void Entity::setMaxHP(INT32 maxHP) { this->maxHP = maxHP; }

bool Entity::getIsKilled() const { return isKilled; }

void Entity::setIsKilled(bool status) { isKilled = status; }
