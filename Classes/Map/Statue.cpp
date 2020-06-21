#include "Statue.h"

#include "Scene/BattleScene.h"

Label* Statue::getTextLabel() const { return this->textLabel; }

void Statue::setGoldCost(INT32 cost) { this->goldCost = cost; }

INT32 Statue::getGoldCost() const { return this->goldCost; }

INT32 Statue::getStatueType() const { return this->statueType; }

void Statue::setPlayerVisited(bool status) { this->playerVisited = status; }

bool Statue::getPlayerVisited() const { return this->playerVisited; }

bool Statue::init() {
	playerVisited = false;
	//雕像标签
	textLabel = Label::create("", "fonts/Marker Felt.ttf", 22);
	textLabel->setColor(ccc3(255, 255, 255));
	textLabel->setGlobalZOrder(TOP - 1);
	textLabel->setPosition(Point(60, 210));
	textLabel->setVisible(false);

	this->textLabel = textLabel;
	this->addChild(textLabel);

	srand(static_cast<unsigned int>(time(nullptr)));
	this->statueType = rand() % CNT_TYPE;
#ifndef DEBUG
	INT32 sceneNumber = BattleScene::getSceneNumber();
	this->goldCost = 15.0f * std::pow(1.08f, sceneNumber - 1) + rand() % 4;
#endif  // ! DEBUG

	switch (this->statueType) {
	case RESUME_HP:
		textLabel->setString("Resume HP--" + Value(this->goldCost).asString() +
			"Gold");
		this->bindSprite(Sprite::create("Statue//resume_hp.png"), TOP);
		break;
	case ADD_HP:
		textLabel->setString("Add max HP--" + Value(this->goldCost).asString() +
			"G");
		this->bindSprite(Sprite::create("Statue//add_hp.png"), TOP);
		break;
	case DOUBLE_HP:
		textLabel->setString("Double HP--" + Value(this->goldCost).asString() +
			"Gold");
		this->bindSprite(Sprite::create("Statue//double_hp.png"), TOP);
		break;
	case ADD_ARMOR:
		textLabel->setString("Add max Armor--" + Value(this->goldCost).asString() +
			"Gold");
		this->bindSprite(Sprite::create("Statue//add_armor.png"), TOP);
		break;
	case DOUBLE_ARMOR:
		textLabel->setString("Double max Armor--" + Value(this->goldCost).asString() +
			"Gold");
		this->bindSprite(Sprite::create("Statue//double_armor.png"), TOP);
		break;
	case DOUBLE_DAMAGE:
		textLabel->setString("Double damage--" +
			Value(this->goldCost).asString() + "Gold");
		this->bindSprite(Sprite::create("Statue//double_damage.png"), TOP);
		break;
	case ADD_MOVE_SPEED:
		textLabel->setString("Add move speed--" +
			Value(this->goldCost).asString() + "Gold");
		this->bindSprite(Sprite::create("Statue//add_move_speed.png"), TOP);
		break;
	case RESUME_MP:
		textLabel->setString("Resume MP--" +
			Value(this->goldCost).asString() + "Gold");
		this->bindSprite(Sprite::create("Statue//resume_mp.png"), TOP);
		break;
	case ADD_MP:
		textLabel->setString("Add max MP--" + Value(this->goldCost).asString() +
			"Gold");
		this->bindSprite(Sprite::create("Statue//add_mp.png"), TOP);
		break;
	}
	return true;
}

void Statue::bindSprite(Sprite* sprite, INT32 layer) {
	//将Node中心和sprite中心重合
	Size size = sprite->getContentSize();
	this->setContentSize(size);               //设置大小
	this->setAnchorPoint(Point(0.5f, 0.5f));  //设置锚点
	setPosition(Point(.0f, .0f));

	//分割精灵
	auto tmpSprite =
		Sprite::create(sprite->getResourceName(), Rect(0, 0, 120, 100));
	tmpSprite->setPosition(Point(60, 150));
	tmpSprite->setGlobalZOrder(layer);
	this->addChild(tmpSprite);

	tmpSprite = Sprite::create(sprite->getResourceName(), Rect(0, 100, 120, 100));
	tmpSprite->setPosition(Point(60, 50));
	tmpSprite->setGlobalZOrder(LayerPlayer - 1);
	this->addChild(tmpSprite);
}

void Statue::giveBuff(Knight* knight) { //人物获得增益
	srand(static_cast<unsigned int>(time(nullptr)));
	INT32 addNum = 3 + rand() % 3;

	switch (this->statueType) {
	case RESUME_HP:
		knight->setHP(knight->getHP() + 2 + rand() % 4);
		break;
	case ADD_HP:
		knight->setMaxHP(knight->getMaxHP() + addNum);
		knight->setHP(knight->getHP() + addNum);
		break;
	case DOUBLE_HP:
		knight->setMaxHP(knight->getMaxHP() << 1);
		knight->setHP(knight->getHP() << 1);
		break;
	case ADD_ARMOR:
		knight->setMaxArmor(knight->getMaxArmor() + addNum);
		knight->setArmor(knight->getArmor() + addNum);
		break;
	case DOUBLE_ARMOR:
		knight->setMaxArmor(knight->getMaxArmor() << 1);
		knight->setArmor(knight->getArmor() << 1);
		break;
	case DOUBLE_DAMAGE:
		knight->setDamageBuff(knight->getDamageBuff() << 1);
		break;
	case ADD_MOVE_SPEED:
		knight->setMoveSpeedBuff(knight->getMoveSpeedBuff() + 1);
		break;
	case RESUME_MP:
		knight->setMP(knight->getMP() + 100);
		break;
	case ADD_MP:
		knight->setMaxMP(knight->getMaxMP() + addNum * 30);
		knight->setMP(knight->getMP() + addNum * 30);
		break;
	}
	//增加金币消耗提示
	auto textLabel =
		Label::create("-" + Value(goldCost).asString() + "Gold", "fonts/Marker Felt.ttf", 20);
	textLabel->setColor(ccc3(255, 255, 255));
	textLabel->setGlobalZOrder(TOP);
	textLabel->setPosition(Point(20, 70));
	knight->addChild(textLabel);

	/*创建一个延时动作*/
	auto fadeOut = FadeOut::create(1.2f);

	auto actions = Sequence::create(fadeOut, RemoveSelf::create(), NULL);
	textLabel->runAction(actions);
}