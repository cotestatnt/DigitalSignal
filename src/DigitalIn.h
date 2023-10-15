#ifndef DIGITALIN_H
#define DIGITALIN_H

#include "Arduino.h"

typedef int IOPinName;
typedef int IOPinMode;

typedef void(*function_cb)(void*);

/*
  A digital input, used for reading the state of a pin
*/
class DigitalIn {

  private:
    enum { IDLE, DEBOUNCED, PRESSED, LONG_CLICK, CLICK, N_CLICK};
    IOPinName   m_pin;
    IOPinMode   m_mode;

    uint32_t    m_longClickTime = 1000;
    uint32_t    m_doubleClickTime = 500;
    uint32_t    m_bounceTime;
    uint32_t    m_deadTime;
    uint32_t    m_lastCheckTime;
    uint32_t    m_pressedDuration;
    uint32_t    m_lastEventTime;

    uint8_t     m_fsmState;
    uint8_t     m_shortPressCnt;
    bool        m_pressEdge;
    bool        m_longPress;

    function_cb fn_press = nullptr;
    function_cb fn_release = nullptr;

  public:

    DigitalIn(IOPinName _pin, IOPinMode _mode = INPUT_PULLUP, uint32_t _time = 70);

    void pressCallback(function_cb fn)  {
      onPressed(fn);
    }
    void releaseCallback(function_cb fn) {
      onReleased(fn);
    }

    void onPressed(function_cb fn);
    void onReleased(function_cb fn);

    /** Set the input pin mode

        @param pull INPUT, INPUT_PULLUP, OUTPUT
    */
    void mode(IOPinMode pull);

    /** Set the long click time different from default

        @param newTime
    */
    void setLongClickTime(uint32_t newTime);

    /** Set the double click time different from default

        @param newTime
    */
    void setDoubleClickTime(uint32_t newTime);

    /** Get current status of output */
    bool isActive() ;

    /** Update current status. Run as much often as possible.  */
    uint8_t update() ;

    /** Check for single debounced click */
    bool click();

    /** Check for double (or more) click (return true on release confgurable) */
    bool doubleClick();

    /** Check for long click (return true on release confgurable)

        @param waitRelease true, false
    */
    bool longClick(bool waitRelease = false);

    /** An operator shorthand for read status
       \sa DigitalIn::read()
       @code
            DigitalIn  button(BUTTON1);
            DigitalOut led(LED1);
            led = button;   // Equivalent to led.write(button.read())
       @endcode
    */
    operator bool() {
      update();
      return isActive() && m_pressedDuration > m_bounceTime;
    }

    operator int() {
      update();
      return isActive() && m_pressedDuration > m_bounceTime;
    }

    /** Get assigned GPIO number */
    int getPin() {
      return m_pin;
    }
};


#endif