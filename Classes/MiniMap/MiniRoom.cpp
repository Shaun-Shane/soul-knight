#include "MiniRoom.h"

void MiniRoom::createRoom(float _x, float _y, Color4F color) { //5 * 5矩阵中的位置
  x = _x, y = _y;
  downLeftX = 5.0f + x * 30, downLeftY = 5.0f + y * 30;
  upRightX = 25.0f + x * 30, upRightY = 25.0f + y * 30;

  for (int dir = 0; dir < CNTDIR; dir++) {
    miniHall[dir] = nullptr;
  }

  this->drawSolidRect(Vec2(downLeftX, downLeftY), Vec2(upRightX, upRightY),
                            color);
  this->setGlobalZOrder(100);
}
