#include <DigitalSignals.h>

#define BTN1 2
#define LED1 LED_BUILTIN
#define LED2 12

DigitalIn   button(BTN1);                     		// Default INPUT_PULLUP, debounce threshold 70ms
DigitalOut  out1(Type::TON, LED1, 1000, 0);   		// Delayed ON timer
DigitalOut  out2(Type::TON_M, LED2, 2000, 5000); 	// Delayed ON timer with memory (reset after 5s)

/*               ______________________________
* input:   _____|                              |________________________
*                >  1000    < _________________
* out1:    __________________|                 |________________________
*                >         2000            < ______________
* out2:    _________________________________|  Toff: 5000  |_________
*/

void setup() {
}

void loop() {
  // Delayed ON timer
  out1.run(button);

  // Delayed ON timer with memory will be automatic cleared after 1000ms
  out2.run(button);
  

  /* Alternative way (more complex, but could be useful to handle)
  if (out2) {
    if (millis() - out2.switchTime() > out2.getTime() + 1000) {
      out2.clear();
    }
  }
  */
}
