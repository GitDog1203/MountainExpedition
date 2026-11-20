#include <M5Cardputer.h>
#include "config.h"
#include "input.h"
#include "level.h"
#include "player.h"
#include "shop.h"
#include "campsite.h"

auto& d = M5Cardputer.Display;
M5Canvas canvas(&d);

GameState gameState = GameState::Shop;
int currentLevelNum = 1;
Level currentLevel;
Player climber;
float cameraX = 0.0f;

uint32_t lastFrameMs = 0;
uint32_t lastRenderMs = 0;
float accumulator = 0.0f;

void setup() {
  Serial.begin(115200);
  auto cfg = M5.config();
  M5Cardputer.begin(cfg, true);

  d.setRotation(0);
  canvas.setPsram(false);
  canvas.createSprite(SCREEN_W, SCREEN_H);

  loadLevel(currentLevel, currentLevelNum);
  initPlayer(climber, currentLevel);
  resetShop();

  lastFrameMs = millis();
  lastRenderMs = millis();
}

void loop() {
  M5Cardputer.update();

  uint32_t now = millis();
  float dt = (now - lastFrameMs) / 1000.0f;
  lastFrameMs = now;
  if (dt > 0.1f) dt = 0.1f;

  accumulator += dt;

  while (accumulator >= FIXED_DT) {
    updateInput();

    if (gameState == GameState::Shop) {
      if (updateShop(climber)) {
        initPlayer(climber, currentLevel);
        gameState = GameState::Playing;
      }
    } else if (gameState == GameState::Playing) {
      updatePlayer(climber, currentLevel, FIXED_DT);

      if (climber.reachedGoal) {
        climber.money += 50;
        climber.stamina = 100.0f;
        climber.inventoryOpen = false;

        if (currentLevelNum >= 3) {
          gameState = GameState::SummitComplete;
        } else {
          resetCampsite();
          gameState = GameState::Campsite;
        }
      }

      cameraX = climber.x - (SCREEN_W * 0.4f);
      float maxCameraX = (MAP_COLS * TILE_SIZE) - SCREEN_W;
      if (cameraX < 0.0f) cameraX = 0.0f;
      if (cameraX > maxCameraX) cameraX = maxCameraX;
    } else if (gameState == GameState::Campsite) {
      if (updateCampsite(climber)) {
        currentLevelNum++;
        loadLevel(currentLevel, currentLevelNum);
        initPlayer(climber, currentLevel);
        gameState = GameState::Playing;
      }
    } else if (gameState == GameState::SummitComplete) {
      if (getInput().confirmPressed) {
        currentLevelNum = 1;
        climber.money = 150;
        climber.rations = 2;
        climber.repairKits = 1;
        climber.warmth = 100.0f;
        climber.hunger = 100.0f;
        climber.stamina = 100.0f;
        climber.gear = 100.0f;
        loadLevel(currentLevel, currentLevelNum);
        initPlayer(climber, currentLevel);
        resetShop();
        gameState = GameState::Shop;
      }
    }

    accumulator -= FIXED_DT;
  }

  if (now - lastRenderMs >= FRAME_TIME_MS) {
    lastRenderMs = now;

    if (gameState == GameState::Shop) {
      drawShop(climber, currentLevelNum, canvas);
    } else if (gameState == GameState::Campsite) {
      drawCampsite(climber, currentLevelNum, canvas);
    } else if (gameState == GameState::SummitComplete) {
      canvas.fillScreen(BLACK);

      canvas.drawPixel(25, 20, WHITE);
      canvas.drawPixel(60, 35, WHITE);
      canvas.drawPixel(185, 22, WHITE);
      canvas.drawPixel(215, 40, WHITE);

      canvas.fillTriangle(40, 120, 120, 26, 200, 120, DARKGREY);
      canvas.fillTriangle(94, 56, 120, 26, 146, 56, WHITE);

      canvas.drawFastVLine(120, 12, 14, WHITE);
      canvas.fillTriangle(121, 12, 134, 16, 121, 20, GREEN);

      canvas.fillRect(108, 16, 8, 10, RED);
      canvas.fillRect(109, 12, 6, 4, ORANGE);

      canvas.setTextSize(1);
      canvas.setTextColor(YELLOW);
      canvas.setCursor(48, 66);
      canvas.print("SUMMIT REACHED! YOU WIN!");

      canvas.setTextColor(CYAN);
      canvas.setCursor(44, 82);
      canvas.printf("Final Cash: $%d  Gear: %d%%", climber.money, (int)climber.gear);

      canvas.setTextColor(WHITE);
      canvas.setCursor(42, 104);
      canvas.print("Press ENTER to Play Again");
    } else {
      canvas.fillScreen(BLACK);
      drawLevel(currentLevel, canvas, cameraX);
      drawPlayer(climber, canvas, cameraX);
      drawPlayerHUD(climber, canvas);

      if (climber.isDead) {
        canvas.fillRect(30, 45, 180, 45, BLACK);
        canvas.drawRect(30, 45, 180, 45, RED);
        canvas.setTextColor(RED);
        canvas.setCursor(55, 54);
        canvas.print("EXPEDITION FAILED!");
        canvas.setTextColor(WHITE);
        canvas.setCursor(46, 70);
        canvas.print("Press ENTER to Retry");

        if (getInput().confirmPressed) {
          climber.warmth = 100.0f;
          climber.hunger = 100.0f;
          climber.stamina = 100.0f;
          climber.gear = 100.0f;
          initPlayer(climber, currentLevel);
        }
      }
    }

    canvas.pushSprite(0, 0);
  }
}