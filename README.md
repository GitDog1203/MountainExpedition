# Mountain Expedition

Mountain Expedition is a mountain climbing game I made for the M5Stack Cardputer ADV.

You start at basecamp, buy supplies, and make your way through three different mountain stages. Along the way, you have to manage your food, warmth, stamina, and gear while trying to reach the summit.

## Features

- 3 mountain stages: Foothills, Ice Ridge, and Final Summit Push
- Platforming, jumping, and ladder climbing
- Ration management
- Gear durability and repair kits
- Basecamp shop where you can buy supplies
- Campsites between mountain stages
- Backpack inventory system
- Survival stats: Warmth, Hunger, Stamina, and Gear Health
- Summit victory screen
- Expedition failure and stage retry system

## Gameplay

The expedition has a few different parts:

1. **Basecamp:** Start with $150 and buy Trail Rations and Repair Kits.
2. **Foothills:** Climb to the first flag and earn $50.
3. **First Campsite:** Rest, eat rations, and repair your gear.
4. **Ice Ridge:** Make your way across gaps and avoid ice spikes.
5. **Second Campsite:** Prepare your supplies for the final climb.
6. **Final Summit Push:** Climb the ladder network to reach the summit.
7. **Summit:** Complete the expedition and see your final stats.

If you fall into a pit or run out of survival stats, your expedition fails. You can retry the current stage.

## Controls

| Key | Action |
|---|---|
| Movement keys | Move and climb |
| Jump key | Jump |
| E | Open backpack |
| ENTER | Confirm, use supplies, or continue |

## Installation

You can download the latest precompiled firmware from the [Releases](../../releases) page.

### Flashing the Firmware

The release includes a merged firmware `.bin` file for the M5Stack Cardputer ADV.

1. Download the merged `.bin` file from the latest release.
2. Connect your Cardputer ADV to your computer using USB-C.
3. Open an ESP32 firmware flashing tool that supports ESP32-S3.
4. Select **ESP32-S3** as the chip.
5. Select the downloaded merged firmware file.
6. Set the flash address to `0x0`.
7. Start flashing and wait for it to finish.
8. Restart your Cardputer ADV.

**If your Cardputer doesn't enter download mode:**

1. Turn the side power switch OFF.
2. Hold down the G0 button.
3. Power the device back on while holding G0.
4. Release G0 after the device powers on.

Then connect it to your computer and try flashing again.

## Source Code

The project is built with Arduino IDE and split into multiple files:

- `config.h` – Game settings, screen dimensions, and game states.
- `input.h` / `input.cpp` – Keyboard controls.
- `level.h` / `level.cpp` – Mountain maps, terrain, and collisions.
- `player.h` / `player.cpp` – Player movement, survival stats, and rendering.
- `shop.h` / `shop.cpp` – Basecamp shop and supplies.
- `campsite.h` / `campsite.cpp` – Resting, backpack management, and repairs.
- `MountainExpedition.ino` – Main game loop and state management.

## Hardware

- M5Stack Cardputer ADV

Made as a project to learn more about programming, game development, and working with the M5Stack platform!
Used ChatGPT to make the README look nice.
Used Gemini to help debug and write parts of code.
