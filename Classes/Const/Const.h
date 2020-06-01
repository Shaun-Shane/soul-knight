#ifndef _CONST_H_
#define _CONST_H_
#include "cocos2d.h"

constexpr INT32 CNTDIR = 4;

enum {RIGHT, UP, LEFT, DOWN};
constexpr INT32 DIRX[CNTDIR] = {1, 0, -1, 0};  // right up left down
constexpr INT32 DIRY[CNTDIR] = {0, 1, 0, -1};  // right up left down

constexpr INT32 FLOORWIDTH = 40;
constexpr INT32 FLOORHEIGHT = 40;

constexpr INT32 WALLWIDTH = 40;
constexpr INT32 WALLHEIGHT = 60;

constexpr INT32 SIZEHALL = 7;

constexpr INT32 CENTERDIS = 39 * 40;

constexpr INT32 SIZECENTERDIS = 39; //两房间相距格子数

constexpr INT32 SIZEROOM = 19;

constexpr INT32 LayerPlayer = 6;

constexpr INT32 TOP = 100;
#endif  // !_CONST_H_
