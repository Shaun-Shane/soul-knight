#include "Statue.h"

#include "Scene/BattleScene.h"

bool Statue::init() {
  playerVisited = false;
  //雕像标签
  textLabel = Label::create("", "fonts/Marker Felt.ttf", 22);
  textLabel->setColor(ccc3(255, 255, 255));
  textLabel->setGlobalZOrder(TOP);
  textLabel->setPosition(Point(60, 210));

  this->textLabel = textLabel;
  this->addChild(textLabel);

  srand(static_cast<unsigned int>(time(nullptr)));
  this->statueType = rand() % CNT_TYPE;

  INT32 sceneNumber = BattleScene::getSceneNumber();
  this->goldCost = 15.0f * std::pow(1.08f, sceneNumber - 1) + rand() % 4;

  switch (this->statueType) {
    case RESUME_HP:
      textLabel->setString("Add HP--" + Value(this->goldCost).asString() +
                           "G");
      break;
    case ADD_HP:
      textLabel->setString("Add max HP--" +
                           Value(this->goldCost).asString() + "G");
      break;
    case DOUBLE_HP:
      textLabel->setString("Double HP--" + Value(this->goldCost).asString() +
                           "G");
      break;
    case ADD_ARMOR:
      textLabel->setString("Add Armor--" + Value(this->goldCost).asString() +
                           "G");
      break;
    case DOUBLE_ARMOR:
      textLabel->setString("Double Armor--" + Value(this->goldCost).asString() +
                           "G");
      break;
    case DOUBLE_DAMAGE:
      textLabel->setString("Double damage--" + Value(this->goldCost).asString() +
                           "G");
      break;
    case ADD_MOVE_SPEED:
      textLabel->setString("Add move speed--" + Value(this->goldCost).asString() +
                           "G");
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

Label* Statue::getTextLabel() const { return this->textLabel; }

void Statue::setGoldCost(INT32 cost) { this->goldCost = cost; }

INT32 Statue::getGoldCost() const { return this->goldCost; }

INT32 Statue::getStatueType() const { return this->statueType; }
