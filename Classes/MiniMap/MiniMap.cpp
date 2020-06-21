#include "MiniMap.h"

bool MiniMap::init() {
	for (int x = 0; x < SIZEMTX; x++)
		for (int y = 0; y < SIZEMTX; y++) {
			miniRoom[x][y] = nullptr;
		}

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto backGround = DrawNode::create();
	backGround->drawSolidRect(Vec2(0, 0), Vec2(NIMIMPAWIDTH, NIMIMPAWIDTH),
		Color4F(0.9f, 0.9f, 0.9f, 0.2f));
	this->addChild(backGround);
	backGround->setGlobalZOrder(TOP);

	this->setPosition(Point(visibleSize.width * 0.82f, visibleSize.height * 3 / 5));
	return true;
}
