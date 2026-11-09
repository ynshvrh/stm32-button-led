#include <Arduino.h>

const uint8_t BTN = D2;
const uint8_t LED = D3;

const uint32_t DEBOUNCE_MS   = 30;
const uint32_t LONG_PRESS_MS = 800;
const uint32_t BLINK_MS      = 250;

enum Mode { OFF, ON, BLINK };
Mode mode = OFF;

volatile bool edgeSeen = false;   
bool checking = false;            
bool pressed = false;            
uint32_t lastEdge = 0;
uint32_t pressedAt = 0;

void onEdge() {
  edgeSeen = true;               
}

void setup() {
  pinMode(BTN, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(BTN), onEdge, CHANGE);
}

void loop() {
  uint32_t now = millis();

  
  if (edgeSeen) {
    edgeSeen = false;
    lastEdge = now;
    checking = true;
  }


  if (checking && now - lastEdge >= DEBOUNCE_MS) {
    checking = false;
    bool nowPressed = (digitalRead(BTN) == LOW);

    if (nowPressed && !pressed) {
      pressed = true;
      pressedAt = now;
    } else if (!nowPressed && pressed) {
      pressed = false;
      uint32_t held = now - pressedAt;

      if (held >= LONG_PRESS_MS) {
        mode = (mode == BLINK) ? OFF : BLINK;
      } else {
        mode = (mode == ON) ? OFF : ON;
      }
    }
  }


  switch (mode) {
    case OFF:   digitalWrite(LED, LOW);  break;
    case ON:    digitalWrite(LED, HIGH); break;
    case BLINK: digitalWrite(LED, (now / BLINK_MS) % 2); break;
  }
}