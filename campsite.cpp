#include "config.h"
#include "input.h"
#include "campsite.h"

static const char* campMessage = "Reached Camp! [E: Bag | OK: Next]";

static float  clampCampStat(float val) {
  if (val < 0.0f) return 0.0f;
if (val > 100.0f) return 100.0f;
  return val;
}

void resetCampsite() {
    campMessage = "Reached Camp! [E: Bag | ENTER: Next]";
}

bool updateCampsite(Player& p) {
    const auto& in = getInput();

    if (in.backpackPressed) {
        p.inventoryOpen = !p.inventoryOpen;
        return false;
    }

    if (p.inventoryOpen) {
        static bool prevUp = false;
        static bool prevDown = false;
        if (in.up && !prevUp)     p.invCursor = (p.invCursor + 1) % 2;
        if (in.down && !prevDown) p.invCursor = (p.invCursor + 1) % 2;
        prevUp = in.up;
        prevDown = in.down;

        if (in.confirmPressed) {
            if (p.invCursor == 0) {
                if (p.rations > 0 && p.hunger < 99.0f) {
                    p.rations--;
                    p.hunger = clampCampStat(p.hunger + 40.0f);
                    p.warmth = clampCampStat(p.warmth + 20.0f);
                    campMessage = "Ate Ration! (+Food & +Warmth)";
                } else {
                    campMessage = (p.rations == 0) ? "No rations left!" : "Already full!";
                }
            } else if (p.invCursor == 1) {
                if (p.repairKits > 0 && p.gear < 99.0f) {
                    p.repairKits--;
                    p.gear = clampCampStat(p.gear + 50.0f);
                    campMessage = "Repaired Gear! (+50% Gear)";
                } else {
                    campMessage = (p.repairKits == 0) ? "No repair kits left!" : "Gear already 100%!";
                }
            }
        }
        return false;
    }

    if (in.confirmPressed) {
        return true;
    }

    return false;
}

void drawCampsite(const Player& p, int completedLevel, M5Canvas& target) {
    target.fillScreen(BLACK);

    drawPlayerHUD(p, target);

    target.drawPixel(30, 22, WHITE);
    target.drawPixel(85, 18, WHITE);
    target.drawPixel(155, 25, WHITE);
    target.drawPixel(210, 16, WHITE);
    target.fillRect(0, 96, SCREEN_W, 39, WHITE);

    int tentX = 48;
    int tentY = 48;
    target.fillTriangle(tentX, tentY + 48, tentX + 34, tentY, tentX + 68, tentY + 48, ORANGE);

    target.fillTriangle(tentX + 18, tentY + 48, tentX + 34, tentY + 18, tentX + 50, tentY + 48, BLACK);


    int px = tentX + 29;
    int py = tentY + 34;
    target.fillRect(px, py + 4, 10, 10, RED);
    target.fillRect(px + 1, py, 8, 5, ORANGE);
    target.drawPixel(px + 6, py + 2, BLACK);
    target.drawPixel(px + 8, py + 2, BLACK);

    int fireX = 148;
    int fireY = 92;
    target.fillRect(fireX - 4, fireY + 2, 12, 3, BROWN);
    bool flicker = ((millis() / 180) % 2) == 0;
    target.fillTriangle(fireX - 3, fireY + 2, fireX + 2, fireY - (flicker ? 10 : 7), fireX + 7, fireY + 2, RED);
    target.fillTriangle(fireX - 1, fireY + 2, fireX + 2, fireY - (flicker ? 6 : 4), fireX + 5, fireY + 2, YELLOW);

    target.setTextSize(1);
    target.setTextColor(CYAN);
    target.setCursor(12, 14);
    target.printf("STAGE %d CAMP  (+$50 Bonus!)", completedLevel);

    target.setTextColor(BLACK);
    target.setCursor(8, 104);
    target.print(campMessage);

    target.setTextColor(DARKGREY);
    target.setCursor(8, 120);
    target.print("[E] Open Backpack   [ENTER] Next Climb");

    drawInventoryOverlay(p, target);
}