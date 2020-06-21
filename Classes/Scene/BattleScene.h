#ifndef _BATTLESCENE_H_
#define _BATTLESCENE_H_

#include "Actor\Knight.h"
#include "Enemy\Enemy.h"
#include "BattleRoom.h"
#include "SafeScene.h"
#include "Hall.h"
#include "Const\Const.h"
#include "MiniMap\MiniMap.h"
#include "MiniMap\MiniRoom.h"
#include "cocos2d.h"
#include <queue>
#include"ui/CocosGUI.h"

USING_NS_CC;
using std::queue;
using std::vector;
using std::string;

class BattleScene : public Scene {
	friend class SafeScene;
	static constexpr INT32 SIZEMTX = 5;
	static constexpr INT32 MAXROOM = 6; // temporarily make it 6

public:
	static Scene* createScene();

	static INT32 getSceneNumber(); //获取场景编号

	static const vector<string>& getVecSceneType(); //获取场景类型容器

	virtual bool init();

	virtual void update(float delta);

	CREATE_FUNC(BattleScene);

	/*回调函数*/
	void menuCloseCallbackEnd(cocos2d::Ref* pSender);

	void menuCloseCallbackSet(cocos2d::Ref* pSender);

private:
	void updatePlayerPos();

	void updatePlayerInfoBar();

	void updateEnemy();

	void updateBoss();

	void updateBossInfoBar();

	void checkEndRoom();

	void initRoom(); //generate room randomly

	void initMiniMap();

	void getToRoom(INT32, INT32, BattleRoom*, queue<BattleRoom*>&);

	void randomGenerate(INT32, INT32);

	void setRoomType();

	void setHallWithWidth(Hall*, DrawNode*, const BattleRoom*, const BattleRoom*);

	void setHallWithHeight(Hall*, DrawNode*, const BattleRoom*, const BattleRoom*);

	void connectRoom(BattleRoom*);

	void updateGold();

	void updateLevel();

private:
	static INT32 battleSceneNumber;
	static vector<string> vecSceneType; //场景类型
	INT32 cntRoom = 0;

private:
	static Knight* knight;
	MiniMap* miniMap = nullptr;

	// temporarily just one kind of map floor and wall
private:
	Vector<Sprite*> vecFloor;
	Vector<Sprite*> vecWall;
	Vector<Sprite*> vecDoor;
	Vector<Enemy*> vecEnemy;
	Vector<Hall*> vecHall;

	BattleRoom* beginRoom = nullptr;
	BattleRoom* endRoom = nullptr;
	BattleRoom* battleRoom[SIZEMTX][SIZEMTX] = { nullptr };  // room

	ui::LoadingBar* BloodLoadingBar;
	ui::LoadingBar* ArmorLoadingBar;
	ui::LoadingBar* MPLoadingBar;

	Label* HPLabel;
	Label* armorLabel;
	Label* MPLabel;

	Sprite* BOSSBloodBg = nullptr;
	ui::LoadingBar* BOSSLoadingBar = nullptr;

	Label* goldLabel;
	Label* gameLevelLabel;
};

#endif
