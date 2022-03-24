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
  button1.update();
}
```

# Introduction and quick start for `DigitalOut`
