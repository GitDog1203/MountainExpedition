#include "input.h"
#include "shop.h"

static int selectedIndex = 0;
static const char* shopMessage = "Stock up before the climb!";
constexpr int SHOP_ITEMS = 4;

void resetShop() {
  selectedIndex = 0;
  shopMessage = "Stock up before the climb!";
}

bool updateShop(Player& p) {
  const auto& in = getInput();

  static bool prevUp = false;
  static bool prevDown = false;
  bool upClick = in.up && !prevUp;
  bool downClick = in.down && !prevDown;
  prevUp = in.up;
  prevDown = in.down;

  if (upClick) {
    selectedIndex = (selectedIndex - 1 + SHOP_ITEMS) % SHOP_ITEMS;
  }
  if (downClick) {
    selectedIndex = (selectedIndex + 1) % SHOP_ITEMS;
  }

  if (in.confirmPressed) {
    if (selectedIndex == 0) {
      if (p.money >= 20) {
        p.money -= 20;
        p.rations++;
        shopMessage = "Bought 1 Trail Ration!";
      } else {
        shopMessage = "Not enough cash!";
      }
    } else if (selectedIndex == 1) {
      if (p.money >= 25) {
        p.money -= 25;
        p.repairKits++;
        shopMessage = "Bought 1 Repair Kit!";
      } else {
        shopMessage = "Not enough cash!";
      }
    } else if (selectedIndex == 2) {
      if (p.money >= 15) {
        p.money -= 15;
        p.warmth = 100.0f;
        p.stamina = 100.0f;
        shopMessage = "Warmed up to 100%!";
      } else {
        shopMessage = "Not enough cash!";
      }
    } else if (selectedIndex == 3) {
      return true;
    }
  }
  return false;
}

void drawShop(const Player& p, int levelNum, M5Canvas& target) {
  target.fillScreen(BLACK);
  target.drawRect(0, 0, SCREEN_W, SCREEN_H, DARKGREY);

  target.setTextSize(1);
  target.setTextColor(ORANGE);
  target.setCursor(8, 6);
  target.printf("BASECAMP SHOP - STAGE %d", levelNum);

  target.setTextColor(GREEN);
  target.setCursor(190, 6);
  target.printf("$%d", p.money);

  target.drawFastHLine(4, 17, SCREEN_W - 8, DARKGREY);

  const char* labels[SHOP_ITEMS] = {
    "Trail Ration (+Foot/Warm)  $20",
    "Repair Kit (+50% Gear)     $25",
    "Hot Cocoa (Full Warm/Stam) $15",
    ">> START CLIMBING <<"
  };
  for (int i = 0; i < SHOP_ITEMS; i++) {
    int y = 24 + (i * 18);
    if (i == selectedIndex) {
      target.fillRect(6, y-2, SCREEN_W - 12, 13, DARKGREY);
      target.setTextColor(YELLOW);
      target.setCursor(10, y);
      target.print("> ");
    } else {
      target.setTextColor(i == 3 ? CYAN : WHITE);
      target.setCursor(10, y);
      target.print(" ");
    }
    target.print(labels[i]);
  }

  target.drawFastHLine(4, 100, SCREEN_W - 8, DARKGREY);
  target.setTextColor(LIGHTGREY);
  target.setCursor(8, 105);
  target.printf("Bag: %d Rations | %d Kits | Gear:%d%%", p.rations, p.repairKits, (int)p.gear);

  target.setTextColor(CYAN);
  target.setCursor(8, 120);
  target.print(shopMessage);
}