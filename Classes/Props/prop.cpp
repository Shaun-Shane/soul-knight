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
