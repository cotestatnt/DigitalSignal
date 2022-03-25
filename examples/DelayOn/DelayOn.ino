#include <DigitalSignals.h>

#define BTN1 2
#define LED1 LED_BUILTIN

DigitalIn   button(BTN1);                     // Default INPUT_PULLUP, debounce threshold 70ms
DigitalOut  out1(OutType::TON, LED1, 1000);   // Delayed ON timer
DigitalOut  out2(OutType::TON_M, LED2, 2000); // Delayed ON timer with memory

/*               ______________________________
* input:   _____|                              |________________________
*                >  1000    < _________________
* out1:    __________________|                 |________________________
*                >         2000            < ______________
* out2:    _________________________________|  Toff: 1000  |_________
*/

void setup() {
  Serial.begin(115200);
  Serial.println("Delay ON example");
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop() {
  // Delayed ON timer
  out1.run(button);

  // Delayed ON timer with memory will be automatic cleared after 1000ms
  out2.run(button, 1000);
  
  /* Alternative way (more complex, but could be useful to handle)
  if (out2) {
    if (millis() - out2.switchTime() > out2.getTime() + 1000) {
      out2.clear();
    }
  }
  */
}
