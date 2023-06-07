
#include "DigitalOut.h"

DigitalOut::DigitalOut(OutType type, uint32_t time) {
  m_type = type;
  m_setTime = time;
  m_offTime = 0;
}

DigitalOut::DigitalOut(OutType type, uint8_t pin, uint32_t time, bool activeLow) {
  m_type = type;
  m_pin = pin;
  m_setTime = time;
  m_activeLow = activeLow;
  m_offTime = 0;
  m_isgpio = true;
  if (activeLow) {
    // Avoid pin high at boot
    digitalWrite(m_pin, HIGH);
  }
  pinMode(m_pin, OUTPUT);
  reset();
}

// Delay OFF
void DigitalOut::runTOFF (bool input) {
  if (input && !m_memoryFlag) {
    m_memoryFlag = true;
    m_activeTime = millis();
    set();
  }
  if (millis() - m_activeTime > m_setTime ) {
    m_memoryFlag = false;
    reset();
  }
}

// Delay ON without memory
void DigitalOut::runTON (bool input) {
  if (input && !m_memoryFlag) {
    m_memoryFlag = true;
    m_activeTime = millis();
  }

  if (millis() - m_activeTime > m_setTime && m_memoryFlag ) {
    set();
  }
  else {
    reset();
  }
  if (!input) {
    m_memoryFlag = false;
  }
}

// Delay ON with memory
void DigitalOut::runTON_M (bool input, uint32_t offTime) {
  if (offTime)
    m_offTime = offTime;
  if (input && !m_memoryFlag) {
    m_memoryFlag = true;
    m_activeTime = millis();
  }
  if (millis() - m_activeTime > m_setTime && m_memoryFlag ) {
    set();
  }
  else {
    reset();
  }
  // m_trigger must be cleared manually or m_offTime must be != 0
  if (m_offTime != 0 && (millis() - m_activeTime) > (m_setTime + m_offTime)) {
    clear();
  }
}

// Blink output n-times. If n_blink == 0, will blink while input is true
uint8_t DigitalOut::blink (bool input, uint8_t n_blink) {
  if (n_blink == 0) {
    if (input) {
      m_blinkCount = -1;
      m_memoryFlag = true;
    }
    else {
      m_blinkCount = 0;
      m_memoryFlag = false;
    }
  }

  if (input && !m_memoryFlag && n_blink) {
    m_memoryFlag = true;
    m_blinkCount = 0;
  }

  if (m_blinkCount < n_blink && m_memoryFlag) {
    if (millis() - m_activeTime > m_setTime ) {
      m_activeTime = millis();
      if (n_blink && m_state)
        m_blinkCount++;
      m_state = !m_state;
    }
  }

  if (m_blinkCount >= n_blink && !input) {
    m_blinkCount = 0;
    m_memoryFlag = false;
    m_state = false;
  }
  digitalWrite(m_pin, m_state);
  return m_blinkCount;
}

// Set output state
void DigitalOut::write(bool val) {
  val ? set() : reset();
}


void DigitalOut::clear() {
  m_memoryFlag = false;
}

void DigitalOut::set() {
  if (m_isgpio)
    digitalWrite(m_pin, m_activeLow ? LOW : HIGH);

  if (!m_lastState && fn_rise != nullptr )
    fn_rise();

  m_state = true;
  m_lastState = m_state;
}

void DigitalOut::reset() {
  if (m_isgpio)
    digitalWrite(m_pin, m_activeLow ? HIGH : LOW);

  if (m_lastState && fn_fall != nullptr)
    fn_fall();

  m_state = false;
  m_lastState = m_state;
}

void DigitalOut::run(bool input, uint32_t param) {
  switch (m_type) {
    case OutType::TOFF:
      runTOFF(input);
      break;
    case OutType::TON:
      runTON(input);
      break;
    case OutType::TON_M:
      runTON_M(input, param);
      break;
    case OutType::BLINK:
      blink(input, param);
      break;
    default:
      break;
  }
}

/* Return the time when output switch active */
uint32_t DigitalOut::switchTime() {
  return m_activeTime;
}

/* Return the preset time */
uint32_t DigitalOut::getTime() {
  return m_setTime;
}

/* Update current preset time */
void DigitalOut::setTime(uint32_t time) {
  m_setTime = time;
}

/* Set off time for auto-reset delay-on timer */
void DigitalOut::setOffTime(uint32_t time) {
  m_offTime = time;
}

/* Return actual state */
int DigitalOut::read() {
  return m_state;
}