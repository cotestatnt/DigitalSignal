#include <DigitalSignals.h>

#define BTN1 2
#define LED1 LED_BUILTIN
#define LED2 12

DigitalIn   button(BTN1);                      // Default INPUT_PULLUP, debounce threshold 70ms
DigitalOut  out1(OutType::BLINK, LED1, 500);   // Blink with 500ms interval
DigitalOut  out2(OutType::BLINK, LED2, 250);   // Blink with 250ms interval (for 5 times)

void setup() {
}

void loop() {

  // Blink while button == true (button remain pushed)
  out1.run(button);

  // Blink for 5 times and then stop
  out2.run(button, 5);
}
