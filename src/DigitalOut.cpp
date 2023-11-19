
#include "DigitalOut.h"

// Boolean variable
DigitalOut::DigitalOut(Type type, uint32_t time1, uint32_t time2) {
  DigitalOut(type, -1, time1, time2, false);
}

// Digital output
DigitalOut::DigitalOut(Type type, const int8_t pin, uint32_t time1, uint32_t time2, bool activeLow) {

  // If pin != -1 output is tied to GPIO
  if (pin > -1) {
    m_pin = pin;
    m_activeLow = activeLow;

    // Avoid out active at boot
    if (activeLow ) {
      digitalWrite(m_pin, HIGH);
    }
    pinMode(m_pin, OUTPUT);
  }

  // Set the type of timer
  m_type = type;
  m_onTime = time1;
  m_offTime = time2;

  switch (m_type) {
    // This kind of timer, need memory flag active
    case Type::TON_M:
    case Type::TON_TOFF:
      m_memory = true;
      break;
    // If Blink, treat times as duration and not as delay
    case Type::BLINK:
      m_onTime = time2;
      m_offTime = time1;
      break;
    case Type::TOFF:
      if (time2 == 0) {
        m_offTime = time1;
        m_onTime = 0;
      }
      m_type = Type::TON_TOFF;
      break;
    default:
      break;
  }
}



void DigitalOut::run(bool input, int count) {

  // Input signal edge
  if (m_lastInput != input) {
    m_lastInput = input;
    m_blinkCount = 0;
  }

  // static int lastState = -1;
  // if (lastState != m_runState) {
  //   lastState = m_runState;
  //   Serial.println(m_runState);
  // }

  switch (m_runState) {
    case RunStates::OFF :
      m_waitOff = false;
      if (input) {
        m_activeTime = millis();
        m_runState = RunStates::DELAY_ON;

        // If Blink, set out ON immediately on input level HIGH
        if (m_type == Type::BLINK) {
          m_runState = RunStates::ON;
        }
      }
      // Reset after last blink (do nothing if count == 0)
      if ((m_blinkCount >= count) && (m_type == Type::BLINK) && (count != 0)) {
        m_activeTime = reset();
      }
      break;

    case RunStates::DELAY_ON :
      if (millis() - m_activeTime >= m_onTime) {
        if (input || m_memory)
          m_runState = RunStates::ON;
        else
          m_runState = RunStates::OFF;
      }
      break;

    case RunStates::ON :
      m_activeTime = set();
      m_runState = RunStates::DELAY_OFF;
      break;

    case RunStates::DELAY_OFF :
      // If SR type, no delay off needed. Output must be resetted manually
      if (m_type == Type::SR) {
        return;
      }
      // If BLINK type, go back to DELAY_ON state
      if ((m_type == Type::BLINK) && (millis() - m_activeTime >= m_offTime)) {
        m_activeTime = reset();
        m_runState = RunStates::DELAY_ON;

        // Increase blink counter (do nothing if count == 0)
        if ((++m_blinkCount >= count) && count ) {
          m_runState = RunStates::OFF;
        }
        return;
      }

      // Reset output
      if (input == false) {
        if ((m_type == Type::TON_TOFF) || (m_type == Type::TON_M && m_offTime)) {
          if (m_waitOff == false) {
            m_waitOff = true;
            m_activeTime = millis();
            break;
          }
          if (millis() - m_activeTime > m_offTime) {
            m_activeTime = reset();
            m_runState = RunStates::OFF;
          }
        }
      }
      break;
  }
}

// Blink output n-times. Will blink while input is true
bool DigitalOut::blink(bool input, int count) {
  run (input, count);
  return (m_blinkCount < count);
}


// Set output state
void DigitalOut::write(bool val) {
  val ? set() : reset();
}

uint32_t DigitalOut::set() {
  if (m_pin > -1) {
    digitalWrite(m_pin, m_activeLow  ? LOW : HIGH);
  }
  m_isActive = true;

  // Execute callback function on rising edge
  if (!m_lastState) {
    m_lastState = true;
    if (fn_rise != nullptr) {
      fn_rise(this);
    }
  }
  return millis();
}

uint32_t DigitalOut::reset() {
  if (m_pin > -1) {
    digitalWrite(m_pin,  m_activeLow ? HIGH : LOW);
  }
  m_isActive = false;

  // Execute callback function on falling edge
  if (m_lastState ) {
    m_lastState = false;
    if (fn_fall != nullptr) {
      fn_fall(this);
    }
  }
  m_runState = RunStates::OFF;
  return millis();
}

/* Return the time when output switch active */
uint32_t DigitalOut::switchTime() {
  return m_activeTime;
}

/* Update current preset time */
void DigitalOut::setTime(uint32_t time1, uint32_t time2) {
  m_onTime = time1;
  m_offTime = time2;
}


/* Return actual state */
int DigitalOut::read() {
  return m_isActive;
}