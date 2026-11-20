#pragma once
#include <M5Cardputer.h>
#include "player.h"

void resetCampsite();
bool updateCampsite(Player& p);
void drawCampsite(const Player& p, int completedLevel, M5Canvas& target);