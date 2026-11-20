#include <M5Cardputer.h>
#include "config.h"
#include "input.h"
#include "player.h"

constexpr float MOVE_SPEED = 75.0f;
constexpr float CLIMB_SPEED = 55.0f;
constexpr float GRAVITY = 420.0f;
constexpr float JUMP_VELOCITY = -185.0f;

static float clampStat(float val) {
  if (val < 0.0f) return 0.0f;
  if (val > 100.0f) return 100.0f;
  return val;
}

static bool collidesWithSolid(const Level& lvl, float x, float y, int w, int h) {
  float left = x + 1;
  float right = x + w - 2;
  float top = y + 1;
  float bottom = y + h - 1;

  return isSolidTile(getTileAtPixel(lvl, left, top)) || isSolidTile(getTileAtPixel(lvl, right, top)) || isSolidTile(getTileAtPixel(lvl, left, bottom)) || isSolidTile(getTileAtPixel(lvl, right, bottom));
}

void initPlayer(Player& p, const Level& lvl) {
  p.x = lvl.spawnX;
  p.y = lvl.spawnY - p.height;
  p.vx = 0.0f;
  p.vy = 0.0f;
  p.onGround = true;
  p.onLadder = false;
  p.facingRight = true;
  p.inventoryOpen = false;
  p.invCursor = 0;
  p.hazardCooldown = 0.0f;
  p.isDead = false;
  p.reachedGoal = false;
}

void updatePlayer(Player& p, const Level& lvl, float dt) {
  if (p.isDead) return;
  const auto& in = getInput();

  if (p.hazardCooldown > 0.0f) p.hazardCooldown -= dt;

  float centerX = p.x + p.width * 0.5f;
  bool touchingLadder = isClimbableTile(getTileAtPixel(lvl, centerX, p.y + p.height * 0.5f)) || isClimbableTile(getTileAtPixel(lvl, centerX, p.y + p.height - 2.0f));

  if (!touchingLadder) {
    p.onLadder = false;
  } else if (in.up || in.down) {
    p.onLadder = true;
  }
  p.vx = 0.0f;
  if (in.left) {
    p.vx = -MOVE_SPEED;
    p.facingRight = false;
  }
  if (in.right) {
    p.vx = MOVE_SPEED;
    p.facingRight = true;
  }

  // 3. Vertical movement & Stamina cost
  bool canExert = (p.stamina > 5.0f);
  if (p.onLadder) {
    p.vy = 0.0f;
    float speed = canExert ? CLIMB_SPEED : (CLIMB_SPEED * 0.45f);
    if (in.up) {
      p.vy = -speed;
      p.stamina -= 14.0f * dt;
    }
    if (in.down) {
      p.vy = speed;
      p.stamina -= 8.0f * dt;
    }
    if (in.jumpPressed && canExert) {
      p.onLadder = false;
      p.vy = JUMP_VELOCITY;
      p.stamina -= 12.0f;
    }
  } else {
    if (in.jumpPressed && p.onGround && canExert) {
      p.vy = JUMP_VELOCITY;
      p.onGround = false;
      p.stamina -= 12.0f;
    }
    p.vy += GRAVITY * dt;
  }

  // Recover stamina when resting or walking on solid ground
  if (!p.onLadder && p.onGround) {
    float regen = (p.vx == 0.0f) ? 26.0f : 14.0f;
    p.stamina += regen * dt;
  }

  // 4. Move X first & resolve wall collisions
  float nextX = p.x + p.vx * dt;
  if (!collidesWithSolid(lvl, nextX, p.y, p.width, p.height)) {
    p.x = nextX;
  }

  // 5. Move Y second & resolve floor/ceiling/ladder-top collisions
  float nextY = p.y + p.vy * dt;
  p.onGround = false;

  bool landedOnLadderTop = false;
  if (!p.onLadder && p.vy > 0.0f && !in.down) {
    float footY = nextY + p.height;
    float prevFootY = p.y + p.height - 2.0f;
    bool footOnLadder = isClimbableTile(getTileAtPixel(lvl, p.x + 2, footY)) || isClimbableTile(getTileAtPixel(lvl, p.x + p.width - 3, footY));
    bool wasAboveLadder = !isClimbableTile(getTileAtPixel(lvl, p.x + p.width * 0.5f, prevFootY));

    if (footOnLadder && wasAboveLadder) {
      int ladderRow = (int)(footY / TILE_SIZE);
      p.y = (ladderRow * TILE_SIZE) - p.height;
      p.vy = 0.0f;
      p.onGround = true;
      landedOnLadderTop = true;
    }
  }

  if (!landedOnLadderTop) {
    if (!collidesWithSolid(lvl, p.x, nextY, p.width, p.height)) {
      p.y = nextY;
    } else {
      if (p.vy > 0.0f) p.onGround = true;
      p.vy = 0.0f;
    }
  }

  // 6. Check Ice Spike ('^') Hazard Collision
  char footTile = getTileAtPixel(lvl, centerX, p.y + p.height - 3.0f);
  if (footTile == '^' && p.hazardCooldown <= 0.0f) {
    p.gear -= 25.0f;
    p.warmth -= 10.0f;
    p.vy = -110.0f;
    p.onGround = false;
    p.hazardCooldown = 1.0f;
  }

  // 7. Gradual Warmth & Hunger Drain
  p.warmth -= 1.2f * dt;
  p.hunger -= (p.vx != 0.0f || p.onLadder) ? (1.3f * dt) : (0.7f * dt);

  p.warmth = clampStat(p.warmth);
  p.hunger = clampStat(p.hunger);
  p.stamina = clampStat(p.stamina);
  p.gear = clampStat(p.gear);

  if (p.warmth <= 0.0f || p.hunger <= 0.0f || p.gear <= 0.0f) {
    p.isDead = true;
  }
  if (p.y > (MAP_ROWS * TILE_SIZE)) {
    p.isDead = true;
    return;
  }
  char centerTile = getTileAtPixel(lvl, centerX, p.y + p.height * 0.5f);
  if (footTile == 'F' || centerTile == 'F') {
    p.reachedGoal = true;
  }
}

void drawPlayer(const Player& p, M5Canvas& target, float cameraX) {
  int sx = (int)(p.x - cameraX);
  int sy = (int)p.y;

  target.fillRect(sx, sy + 2, p.width, p.height - 4, RED);
  target.fillRect(sx + 1, sy - 2, p.width - 2, 5, ORANGE);
  target.fillRect(sx + 1, sy + p.height - 2, 3, 2, BROWN);
  target.fillRect(sx + p.width - 4, sy + p.height - 2, 3, 2, BROWN);

  int eyeX = p.facingRight ? (sx + p.width - 4) : (sx + 2);
  target.drawPixel(eyeX, sy, BLACK);
}

static void drawMiniBar(M5Canvas& target, int x, char letter, float val, uint16_t color) {
  target.setTextColor(WHITE);
  target.setTextSize(1);
  target.setCursor(x, 0);
  target.print(letter);

  int barX = x + 7;
  int barW = 28;
  int fillW = (int)((val / 100.0f) * barW);
  target.drawRect(barX, 1, barW + 2, 5, DARKGREY);
  if (fillW > 0) {
    target.fillRect(barX + 1, 2, fillW, 3, color);
  }
}

void drawPlayerHUD(const Player& p, M5Canvas& target) {
  target.fillRect(0, 0, SCREEN_W, 7, BLACK);

  drawMiniBar(target, 2, 'W', p.warmth, CYAN);
  drawMiniBar(target, 42, 'H', p.hunger, GREEN);
  drawMiniBar(target, 82, 'S', p.stamina, YELLOW);
  drawMiniBar(target, 122, 'G', p.gear, (p.gear > 30.0f) ? ORANGE : RED);

  target.setTextColor(WHITE);
  target.setCursor(165, 0);
  target.printf("$%d R:%d", p.money, p.rations);
}
void drawInventoryOverlay(const Player& p, M5Canvas& target) {
  if (!p.inventoryOpen) return;

  target.fillRect(24, 30, 192, 68, BLACK);
  target.drawRect(24, 30, 192, 68, ORANGE);

  target.setTextSize(1);
  target.setTextColor(ORANGE);
  target.setCursor(32, 36);
  target.print("BACKPACK [ENTER:Use E:Exit]");

  // Option 0: Trail Ration
  target.setTextColor(p.invCursor == 0 ? YELLOW : WHITE);
  target.setCursor(32, 54);
  target.printf("%s Trail Ration (%d) +Food/+Warm", p.invCursor == 0 ? ">" : " ", p.rations);

  // Option 1: Repair Kit
  target.setTextColor(p.invCursor == 1 ? YELLOW : WHITE);
  target.setCursor(32, 70);
  target.printf("%s Repair Kit   (%d) +50%% Gear", p.invCursor == 1 ? ">" : " ", p.repairKits);
}