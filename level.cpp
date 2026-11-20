#include "level.h"
#include "config.h"

constexpr int TOTAL_LEVELS = 3;

static const char* LEVEL_1[MAP_ROWS] = {
  "........................................",
  "........###H..........................F.",
  "...........H........................####",
  "...........H............H.......##......",
  "...........H...#######..H...##..........",
  "...........H.....#..#...H...............",
  "..S......###.....#..#...H...............",
  "############.....####...################"
};

static const char* LEVEL_2[MAP_ROWS] = {
  "..........#.............................",
  "....H####H#..........................F..",
  "....H....H.....................H########",
  "^^^^H^^..H.....................H........",
  "####H##..H###..................H.........",
  "....H.#..H.....###.............H........",
  "..S.H.#^^^^.........###........H........",
  "########################################"
};

static const char* LEVEL_3[MAP_ROWS] = {
  "........................................",
  "......................................F.",
  ".............................H......####",
  "....................H........H..^^......",
  "............H.......H...^^...H.####.....",
  "......H.....H..^^...H..####..H..........",
  "..S...H....###.####.H........H..........",
  "#######....###......##########.........."
};

void loadLevel(Level& lvl, int levelNum) {
  const char** chosen = LEVEL_1;
  if (levelNum == 2) chosen = LEVEL_2;
  else if (levelNum >= 3) chosen = LEVEL_3;

  lvl.spawnX = 2 * TILE_SIZE;
  lvl.spawnY = 6 * TILE_SIZE;

  for (int r = 0; r < MAP_ROWS; r++) {
    for (int c = 0; c < MAP_COLS; c++) {
      char ch = chosen[r][c];
      if (ch == 'S') {
        lvl.spawnX = c * TILE_SIZE;
        lvl.spawnY = (r + 1) * TILE_SIZE;
        lvl.grid[r][c] = '.';
      } else {
        lvl.grid[r][c] = ch;
      }
    }
  }
}

char getTileAtPixel(const Level& lvl, float x, float y) {
  int col = (int)(x / TILE_SIZE);
  int row = (int)(y / TILE_SIZE);
  if (row >= MAP_ROWS) return '.';
  if (col < 0 || col >= MAP_COLS || row < 0) return '#';
  return lvl.grid[row][col];
}

bool isSolidTile(char tile) {
  return tile == '#';
}

bool isClimbableTile(char tile) {
  return tile == 'H';
}

void drawLevel(const Level& lvl, M5Canvas& target, float cameraX) {
  int startCol = (int)(cameraX / TILE_SIZE);
  int endCol = startCol + (SCREEN_W / TILE_SIZE) + 2;
  if (startCol < 0) startCol = 0;
  if (endCol > MAP_COLS) endCol = MAP_COLS;

  for (int r = 0; r < MAP_ROWS; r++) {
    for (int c = startCol; c < endCol; c++) {
      char tile = lvl.grid[r][c];
      int sx = (int)(c * TILE_SIZE - cameraX);
      int sy = r * TILE_SIZE;

      if (tile == '#') {
        target.fillRect(sx, sy, TILE_SIZE, TILE_SIZE, DARKGREY);
        if (r == 0 || lvl.grid[r - 1][c] != '#') {
          target.fillRect(sx, sy, TILE_SIZE, 3, WHITE);
        }
      } else if (tile == 'H') {
        target.drawFastVLine(sx + 4, sy, TILE_SIZE, ORANGE);
        target.drawFastVLine(sx + 11, sy, TILE_SIZE, ORANGE);
        for (int ry = 3; ry < TILE_SIZE; ry += 4) {
          target.drawFastHLine(sx + 4, sy + ry, 8, ORANGE);
        }
      } else if (tile == '^') {
        target.fillTriangle(sx + 1, sy + TILE_SIZE, sx + 4, sy + 6, sx + 7, sy + TILE_SIZE, CYAN);
        target.fillTriangle(sx + 8, sy + TILE_SIZE, sx + 11, sy + 6, sx + 14, sy + TILE_SIZE, CYAN);
      } else if (tile == 'F') {
        target.drawFastVLine(sx + 4, sy + 2, TILE_SIZE - 2, WHITE);
        target.fillTriangle(sx + 5, sy + 2, sx + 14, sy + 6, sx + 5, sy + 10, GREEN);
      }
    }
  }
}