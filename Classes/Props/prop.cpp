#include "prop.h"

Prop::Prop(){}

Prop::~Prop(){}

bool Prop::init()
{
  return true;
}

INT32 Prop::getPropIndex()
{
  return this->propIndex;
}

void Prop::setPropIndex(INT32 index)
{
  this->propIndex = index;
}

void Prop::useProps(Knight* knight)
{
  switch (this->propIndex)
  {
  case 3:
    knight->setHP(2 + (knight->getHP()));
      break;
  case 4:
    knight->setMP(25 + (knight->getMP()));
    break;
  }
}
