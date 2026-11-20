#include <M5Cardputer.h>
#include "input.h"

static InputState currentInput;

static uint32_t lastJumpMs = 0;
static uint32_t lastBackpackMs = 0;
static uint32_t lastPauseMs = 0;
static uint32_t lastConfirmMs = 0;

static bool triggerWithCooldown(bool rawPressed, uint32_t& lastTriggerMs, uint32_t cooldownMs = 200) {
  uint32_t now = millis();
  if (rawPressed && (now - lastTriggerMs >= cooldownMs)) {
    lastTriggerMs = now;
    return true;
  }
  return false;
}

void updateInput() {
  auto& kb = M5Cardputer.Keyboard;
  auto keys = kb.keysState();

  currentInput.left = kb.isKeyPressed(',');
  currentInput.right = kb.isKeyPressed('/');
  currentInput.up = kb.isKeyPressed(';');
  currentInput.down = kb.isKeyPressed('.');
  currentInput.climb = currentInput.up;

  currentInput.jumpPressed = triggerWithCooldown(kb.isKeyPressed('w') || kb.isKeyPressed('W'), lastJumpMs, 200);
  currentInput.backpackPressed = triggerWithCooldown(kb.isKeyPressed('e') || kb.isKeyPressed('E'), lastBackpackMs, 250);
  currentInput.pausePressed = triggerWithCooldown(kb.isKeyPressed('`'), lastPauseMs, 250);
  currentInput.confirmPressed = triggerWithCooldown(keys.enter, lastConfirmMs, 250);
}

const InputState& getInput() {
  return currentInput;
}