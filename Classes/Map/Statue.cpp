#include "Statue.h"

bool Statue::init() { 
  playerVisited = false;

  srand(static_cast<unsigned int>(time(nullptr)));
  this->statueType = rand() % CNT_TYPE;

  switch (this->statueType) {
    case RESUME_HP:
      break;
    case ADD_HP:
      break;
    case DOUBLE_HP:
      break;
    case ADD_ARMOR:
      break;
    case DOUBLE_ARMOR:
      break;
    case DOUBLE_DAMAGE:
      break;
    case ADD_MOVE_SPEED:
      break;
  }
  // this->label->setString();
  return true; 
}

void Statue::bindSprite(Sprite* sprite, INT32 layer) {

}

Label* Statue::getLabel() const { return this->label; }

void Statue::setGoldCost(INT32 cost) { this->goldCost = cost; }

INT32 Statue::getGoldCost() const { return this->goldCost; }

INT32 Statue::getStatueType() const { return this->statueType; }
