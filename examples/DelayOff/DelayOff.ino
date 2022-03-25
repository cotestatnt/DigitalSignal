#include <DigitalSignals.h>

#define BTN1 2
#define LED1 LED_BUILTIN

DigitalIn   button(BTN1);                        // Default INPUT_PULLUP, debounce threshold 70ms
DigitalOut  output(OutType::TOFF, LED1, 2000);   // Delayed OFF timer

/*               ___________     TOFF = 2000
* input:   _____|           |_______________________________
*                _____________________________________
* output:  _____|                                     |_____
*/

void setup() {
  Serial.begin(115200);
  Serial.println("Delay OFF example");
}

void loop() {
  // Delayed OFF timer
  output.run(button);
}
