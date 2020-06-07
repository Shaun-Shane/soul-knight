#include "prop.h"

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
