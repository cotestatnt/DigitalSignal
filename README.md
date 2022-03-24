# Arduino DigitalSignal library
Arduino Digital Signal let you to configure and control in clear and easy way digital signals with your Arduino board like push buttons, swithes, relay (active low or active high), leds etc etc.

[Configure and control digital INPUT signals with debouncing and different click handlers](https://github.com/cotestatnt/DigitalSignal/edit/main/README.md#introduction-and-quick-start-for-digitalin)
  - capture a debounced single click event 
  - capture a debounced double (or more) click event
  - capture a debounced long click event (on release or after setted time)
  - set function callback for press edge
  - set function callback for release edge

[Configure and control digital OUTPUT signals with different working modes](https://github.com/cotestatnt/DigitalSignal/edit/main/README.md#introduction-and-quick-start-for-digitalout)
  - Set/Reset
  - Delayed OFF timer
  - Delayed ON timer
  - Delayed ON timer with memory
  - Blink with user defined frequency

The ouput controlled can be also "virtualized" using a `bool` variable 

# Introduction and quick start for `DigitalIn`
Creates an instance of the `DigitalIn` class
```C++
DigitalIn button(2, INPUT_PULLUP, 50); // pin number, input type, debounce threshold
```
Read the state of input with an operator shorthand
```C++
if (button) {
  Serial.println("The button is pressed");
}
```
Catch the click type events
```C++
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
```
Use the callback function for edge transition
```C++
void buttonPressed() {
  Serial.println("Button pressed");   
}

void buttonReleased() {
  Serial.println("Button released");   
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  button.pressCallback(buttonPressed);
  button.releaseCallback(buttonReleased);
}

void loop() { 
  button.update();
}
```

# Introduction and quick start for `DigitalOut`
Creates an instance of the `DigitalOut` class
```C++
DigitalOut output(OutType::SR, 13, 0, true);  // Set/Reset with active low out (tipical relay board)
DigitalOut output(OutType::BLINK, 13, 500);   // Blink pin (on condition) (type, pin, set time)
DigitalOut output(OutType::TOFF, 13, 2000);   // Delayed OFF timer
DigitalOut output(OutType::TON, 13, 2000);    // Delayed ON timer
DigitalOut output(OutType::TON_M, 13, 2000);  // Delayed ON timer with memory (this timer has to be cleared manually)
```
Read and write the state of output with an operator shorthand
```C++
DigitalOut out1(OutType::TON, 13, 2000); 
DigitalOut out2(OutType::SR, 12);   
DigitalIn input(2); // Default INPUT_PULLUP, debounce threshold 70ms

.......
out1.run(input);

if (out1) {
  Serial.println("The out1 is active");
  out2 = true;
}
```

Delay ON with memory timed auto-reset
```C++
DigitalOut output(OutType::TON_M, 13, 2000);  
DigitalIn input(2); // Default INPUT_PULLUP, debounce threshold 70ms

// When input == true, the timer will start. 
// After setted time, output become true and then back to false after 1000 ms
.......
output.run(input, 1000);    

/* Alternative way (more complex, but could be useful to handle something)
if (output) {
  if (millis() - output.switchTime() > output.getTime() + 1000) {
    output.clear();
  }
}
*/
```

Blink n times or continuosly (while input == true)
```C++
DigitalOut blink1(OutType::BLINK, 13, 500);  
DigitalOut blink2(OutType::BLINK, 12, 500);  
DigitalIn input(2); 

.......
blink1.run(input, 5);    // This will blink 5 times (with memory)
blink2.run(input);       // This will blink while input is true
```
