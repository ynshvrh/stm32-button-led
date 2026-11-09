# stm32-button-led

A small learning project for STM32 (Nucleo C031C6): a button controls an LED using **interrupts**, with **debouncing** and a **state machine**. It runs in the [Wokwi](https://wokwi.com) simulator, so no physical hardware is required.

> 🔗 Wokwi simulation: https://wokwi.com/projects/480254999215896577

## What it does

| Action | Result |
|---|---|
| Short press (< 0.8 s) | LED turns on / off |
| Long press (≥ 0.8 s) | blink mode turns on / off |

Modes: `OFF` → `ON` → `BLINK` (switched according to the button presses).

## Wiring

| Component | Pin |
|---|---|
| Button | `D2` and `GND` (internal pull-up, `INPUT_PULLUP`) |
| LED (through a 220 Ω resistor) | `D3` and `GND` |

The full circuit for the simulator is in [`diagram.json`](diagram.json).

## How it works

- **The interrupt only signals.** The ISR sets a `volatile bool` flag; all the logic runs in `loop()`.
- **Debouncing without `delay()`.** The button state is read once 30 ms have passed since the last edge.
- **Press duration** is measured with `millis()`, which is how short and long presses are told apart.
- **State machine** (`OFF / ON / BLINK`) instead of nested `if` branches.
- Blinking is non-blocking too: the LED state is derived from `millis() / 250`.

The parameters (`DEBOUNCE_MS`, `LONG_PRESS_MS`, `BLINK_MS`) can be changed at the top of `src/main.cpp`.

## Running

Requirements: VS Code with the **PlatformIO IDE** and **Wokwi Simulator** extensions (free Wokwi license: `F1` → *Wokwi: Request a New License*).

1. Clone the repository and open the folder in VS Code.
2. Build the firmware: `pio run` (or `Ctrl+Alt+B`).
3. `F1` → **Wokwi: Start Simulator**.
4. Click the button in the simulation window.

After changing the code, rebuild the project before restarting the simulator.

## Structure

```
src/main.cpp      firmware source
platformio.ini    PlatformIO configuration (board, framework)
diagram.json      circuit for Wokwi
wokwi.toml        firmware paths for the simulator
```

## Ideas for further development

- an SOS mode (three short, three long);
- rewrite from the Arduino API to HAL (`HAL_GPIO_EXTI_Callback`);
- run it on a real Nucleo board.