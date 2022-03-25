#include <DigitalSignals.h>

#define BTN1 2
#define BTN2 3

DigitalIn   button1(BTN1, INPUT, 50);    // INPUT (pull-down) debounce threshold 50ms
DigitalIn   button2(BTN2, INPUT, 50);    // INPUT (pull-down) debounce threshold 50ms

void setup() {
  Serial.begin(115200);
  Serial.println();
  button1.setDoubleClickTime(600);
  button1.setLongClickTime(1200);
}

void loop() {

  if (button2) {
    // The shorthand don't need update() to be called
    Serial.println("A click from push button 2");
    delay(100);
  }

  button1.update();
  if (button1.click()) {
    Serial.println("A single click");
  }

  if (button1.doubleClick()) {
    Serial.println("A double click");
  }

  if (button1.longClick()) {
    Serial.println("A long click");
  }

  if (button1.longClick(true)) {
    Serial.println("A long click on button release");
  }
}