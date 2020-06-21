#include "MiniRoom.h"

void MiniRoom::createRoom(float _x, float _y, Color4F color) { //5 * 5矩阵中的位置
	this->color = GREY;

	x = _x, y = _y;
	downLeftX = 5.0f + x * 30, downLeftY = 5.0f + y * 30;
	upRightX = 25.0f + x * 30, upRightY = 25.0f + y * 30;


	for (int dir = 0; dir < CNTDIR; dir++) {
		miniHall[dir] = nullptr;
	}

	this->drawSolidRect(Vec2(downLeftX, downLeftY), Vec2(upRightX, upRightY),
		color);
	this->setGlobalZOrder(TOP);

#ifndef DEBUG
	this->setVisible(false);
#endif  // ! 
}

void MiniRoom::setColorWhite() {
	if (color == WHITE) return;
	this->setVisible(true);
	this->clear();
	this->drawSolidRect(Vec2(downLeftX, downLeftY), Vec2(upRightX, upRightY),
		Color4F(1.0f, 1.0f, 1.0f, 0.5f));
	color = WHITE;
}


void MiniRoom::setColorGrey() {
	if (color == GREY) return;
	this->clear();
	this->drawSolidRect(Vec2(downLeftX, downLeftY), Vec2(upRightX, upRightY),
		Color4F(0.9f, 0.9f, 0.9f, 0.3f));
	color = GREY;
}



