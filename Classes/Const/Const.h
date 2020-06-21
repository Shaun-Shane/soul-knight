#ifndef _CONST_H_
#define _CONST_H_
#include "cocos2d.h"

constexpr INT32 CNTDIR = 4;

enum Direction { RIGHT, UP, LEFT, DOWN };
constexpr INT32 DIRX[CNTDIR] = { 1, 0, -1, 0 };  // right up left down
constexpr INT32 DIRY[CNTDIR] = { 0, 1, 0, -1 };  // right up left down

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

constexpr float PI = 3.1415926f;

enum RoomType { BEGIN, END, NORMAL, BOSS, WEAPON, PROP/*道具*/ };

enum StatueType {
	RESUME_HP, //恢复生命 free
	ADD_HP,   //增加最大生命值
	DOUBLE_HP, //生命值翻倍
	ADD_ARMOR, //增加最大护甲
	DOUBLE_ARMOR, //护甲翻倍
	DOUBLE_DAMAGE, //双倍伤害
	ADD_MOVE_SPEED, //增加移动速度
	RESUME_MP,  //恢复法力值
	ADD_MP //增加最大法力值
};
#endif  // !_CONST_H_
