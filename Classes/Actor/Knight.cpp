#include "Knight.h"

Knight::Knight() : Entity(4, 5, 1.5f), armor(5), MP(5) {}

Knight::~Knight() {}

bool Knight::init() { return true; }

