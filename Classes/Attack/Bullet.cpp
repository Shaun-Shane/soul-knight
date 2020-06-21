#include "Bullet.h"

Bullet::Bullet() {}

Bullet::~Bullet() {}

bool Bullet::init() { return true; }

Vec2 Bullet::getBulletSpeed() { return this->bulletSpeed; }

void Bullet::setBulletSpeed(Vec2 speed) { this->bulletSpeed = speed; }

bool Bullet::isInScreen() { return true; }

void Bullet::setAttack(INT32 attack) { this->attack = attack; }

INT32  Bullet::getAttack() { return this->attack; }

void Bullet::showEffect(Point pos, Hall* hall) {
	auto effectCircle = DrawNode::create();
	effectCircle->drawSolidCircle(pos, 6.0f, CC_DEGREES_TO_RADIANS(360), 15,
		Color4F(1.0f, 0.0f, .0f, 0.6f));

	effectCircle->setGlobalZOrder(LayerPlayer + 3);
	hall->addChild(effectCircle);

	effectCircle->runAction(Sequence::create(FadeOut::create(0.3f), RemoveSelf::create(), NULL));
}
