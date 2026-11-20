#pragma once
#include <M5Cardputer.h>
#include "level.h"

struct Player {
  float x = 32.0f;
  float y = 80.0f;
  float vx = 0.0f;
  float vy = 0.0f;

  int width = 10;
  int height = 14;

  bool onGround = false;
  bool onLadder = false;
  bool facingRight = true;

  float warmth = 100.0f;
  float hunger = 100.0f;
  float stamina = 100.0f;
  float gear = 100.0f;
   
  int money = 150;
  int rations = 2;
  int repairKits = 1;

  float hazardCooldown = 0.0f;
  bool isDead = false;
  bool reachedGoal = false;

  bool inventoryOpen = false;
  int invCursor = 0;
};

void initPlayer(Player& p, const Level& lvl);
void updatePlayer(Player& p, const Level& lvl, float dt);
void drawPlayer(const Player& p, M5Canvas& target, float cameraX);
void drawPlayerHUD(const Player& p, M5Canvas& target);
void drawInventoryOverlay(const Player& p, M5Canvas& target);