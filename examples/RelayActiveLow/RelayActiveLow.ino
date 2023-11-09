#include <DigitalSignals.h>

#define BTN1 2
#define BTN2 3
#define RLY1 13

DigitalIn   btnSet(BTN1);                            // Default INPUT_PULLUP, debounce threshold 70ms
DigitalIn   btnReset(BTN2);                          // Default INPUT_PULLUP, debounce threshold 70ms
DigitalOut  relay1(Type::SR, RLY1, 0, 0, true);      // Set/Reset relay coil (active on LOW signal)

void setup() {
}

void loop() {

  if (btnSet) {
    // relay1.set();

    // alternative mode:
    relay1 = true;
  }

  if (btnReset) {
    relay1.reset();

    // alternative mode:
    // relay1 = false;
    // relay1 = 0;
    // relay1 = LOW;
  }
}