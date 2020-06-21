#ifndef _MINIMAP_H_
#define _MINIMAP_H_
#include "cocos2d.h"
#include "MiniMap/MiniRoom.h"

USING_NS_CC;

class MiniMap : public Node {
	friend class BattleScene;
	static constexpr INT32 SIZEMTX = 5;
	static constexpr INT32 NIMIMPAWIDTH = 160;
	static constexpr INT32 ROOMWIDTH = 20;

public:
	CREATE_FUNC(MiniMap);

	virtual bool init();

private:
	MiniRoom* miniRoom[SIZEMTX][SIZEMTX] = { nullptr };

};

#endif
