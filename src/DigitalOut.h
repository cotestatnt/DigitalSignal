#ifndef DIGITALOUT_H
#define DIGITALOUT_H
#include "Arduino.h"

namespace Output {
enum class OutType {SR, TOFF, TON, TON_M, BLINK};
}

typedef void(*function_cb)();

using Output::OutType;

class DigitalOut
{
  private:
    uint8_t   m_pin;
    OutType  m_type;

    bool     m_activeLow;
    bool     m_memoryFlag;
    bool     m_state;
    bool     m_lastState;
    bool     m_isgpio;

    uint32_t m_activeTime;
    uint32_t m_setTime;
    uint32_t m_offTime;

    int     m_blinkCount;

    // Delay OFF
    void runTOFF (bool input);

    // Delay ON without memory
    void runTON (bool input);

    // Delay ON wit memory
    void runTON_M (bool input, uint32_t offTime);

    // Blink output n-times. If n_blink == 0, will blink while input is true
    uint8_t blink (bool input, uint8_t n_blink = 0);

    // Set output state
    void write(bool val);

    function_cb fn_rise = nullptr;
    function_cb fn_fall = nullptr;

  public:
    DigitalOut(OutType type, uint32_t time = 0);
    DigitalOut(OutType type, uint8_t pin, uint32_t time = 0, bool activeLow = false);

    void clear();
    void set();
    void reset();
    void run(bool input, uint32_t param = 0);

    /* Return the time when output switch active */
    uint32_t switchTime();

    /* Return the preset time */
    uint32_t getTime();

    /* Update current preset time */
    void setTime(uint32_t time);

    /* Set off time for auto-reset delay-on timer */
    void setOffTime(uint32_t time) ;

    /* Return actual state */
    int read() ;

    /* An operator shorthand for read() */
    operator bool() {
      return read();
    }

    /* A shorthand for write() */
    void operator= (bool value) {
      write(value);
    }

    inline void onRising(function_cb fn) {
      fn_rise = fn;
    }

    inline void onFalling(function_cb fn) {
      fn_fall = fn;
    }

};

#endif