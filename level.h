#pragma once
#include <M5Cardputer.h>
#include "config.h"
struct Level {
  int levelNum = 1;
  const char* name = "Icy Introduction";
  char grid[MAP_ROWS][MAP_COLS + 1];
  float spawnX = 32.0f;
  float spawnY = 80.0f;
};

void loadLevel(Level& lvl, int levelNum);
char getTileAtPixel(const Level& lvl, float worldX, float worldY);
bool isSolidTile(char tile);
bool isClimbableTile(char tile);
void drawLevel(const Level& lvl, M5Canvas& target, float cameraX);