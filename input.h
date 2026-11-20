#pragma once
#include <Arduino.h>


//movment options
struct InputState {
  // held
  bool left = false;
  bool right = false;
  bool up = false;
  bool down = false;
  bool climb = false;
  // one click
  bool jumpPressed = false;
  bool backpackPressed = false;
  bool pausePressed = false;
  bool confirmPressed = false;
};

void updateInput();
const InputState& getInput();