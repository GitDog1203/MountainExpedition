#pragma once
#include <M5Cardputer.h>
#include "player.h"

void resetShop();
bool updateShop(Player& p); // Returns true when you pick "START CLIMB"
void drawShop(const Player& p, int levelNum, M5Canvas& target);