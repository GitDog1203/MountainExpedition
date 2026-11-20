#pragma once  // file only once
#include <Arduino.h>
//added
constexpr int TARGET_FPS = 30;
constexpr uint32_t FRAME_TIME_MS = 1000 / TARGET_FPS;
constexpr float FIXED_DT = 1.0f / 60.0f;

constexpr int SCREEN_W = 240;
constexpr int SCREEN_H = 135;

constexpr int TILE_SIZE = 16;
constexpr int MAP_ROWS = 8;
constexpr int MAP_COLS = 45;

enum class GameState {
  Boot,
  Title,
  Shop,
  Playing,
  Campsite,
  Paused,
  GameOver,
  SummitComplete
};