#ifndef DIGITALOUT_H
#define DIGITALOUT_H
#include "Arduino.h"

namespace Output {
enum class Type {SR, TOFF, TON, TON_M, TON_TOFF, BLINK};
}

namespace  States {
enum RunStates {OFF = 250, DELAY_ON, ON, DELAY_OFF};
}

typedef void(*function_out_cb)(void *arg);

using Output::Type;
using States::RunStates;

class DigitalOut
{
  public:
    // Only boolean variable
    DigitalOut(Type type, uint32_t time1, uint32_t time2);

    // Digital output or boolean variable (pin = -1)
    DigitalOut(Type type, const int8_t pin, uint32_t time1 = 0, uint32_t time2 = 0, bool activeLow = false) ;

    uint32_t set();
    uint32_t reset();

    void run(bool input = false, int count = 0);
    bool blink(bool input = false, int count = 0);

    /* Return the time when output switch active */
    uint32_t switchTime();

    /* Update current preset times */
    void setTime(uint32_t time1, uint32_t time2);

    /* Return actual state */
    int read();

    /* An operator shorthand for read() */
    operator bool() {
      return read();
    }

    /* An operator shorthand for read() */
    operator int() {
      return read();
    }

    /* A shorthand for write() */
    void operator= (bool value) {
      write(value);
    }
    /* A shorthand for write() */
    void operator= (int value) {
      write((bool)value);
    }

    inline void onRising(function_out_cb fn) {
      fn_rise = fn;
    }

    inline void onFalling(function_out_cb fn) {
      fn_fall = fn;
    }

    inline int getPin() {
      return m_pin;
    }

    inline bool isActive() {
      return m_isActive;
    }

  private:
    Type  m_type = Type::SR;
    int8_t   m_pin = -1;
    bool     m_activeLow = false;
    bool     m_memory = false;
    bool     m_isActive = false;
    bool     m_lastState = false;

    uint32_t m_activeTime;
    uint32_t m_offTime;
    uint32_t m_onTime;

    int      m_runState = RunStates::OFF;
    int      m_blinkCount = 0;
    bool     m_lastInput = false;
    bool     m_waitOff = false; 

    // Set output state
    void write(bool val);

    // Callback rising functions
    function_out_cb fn_rise = nullptr;
    function_out_cb fn_fall = nullptr;
};

#endif