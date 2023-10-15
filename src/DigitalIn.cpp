#include "DigitalIn.h"

DigitalIn::DigitalIn(IOPinName _pin, IOPinMode _mode, uint32_t _time) {
  m_lastCheckTime = 0;
  m_pressedDuration = 0;
  m_longPress = false;
  m_pressEdge = false;
  m_shortPressCnt = LONG_CLICK;

  m_bounceTime = _time;
  m_deadTime = 2 * m_bounceTime;
  m_pin = _pin;
  m_mode = _mode;
  pinMode(m_pin, _mode);
}

void DigitalIn::onPressed(function_cb fn) {
  fn_press = fn;
}

void DigitalIn::onReleased(function_cb fn) {
  fn_release = fn;
}

void DigitalIn::mode(IOPinMode pull) {
  m_mode = pull;
  pinMode(m_pin, pull);
}

void DigitalIn::setLongClickTime(uint32_t newTime) {
  m_longClickTime = newTime;
}

void DigitalIn::setDoubleClickTime(uint32_t newTime) {
  m_doubleClickTime = newTime;
}

bool DigitalIn::isActive() {
  if (m_mode == INPUT_PULLUP)
    return !digitalRead(m_pin);
  return digitalRead(m_pin);
}

uint8_t DigitalIn::update() {
  m_fsmState = IDLE;
  uint32_t current_millies = millis();

  if (isActive()) {
    m_pressedDuration += current_millies - m_lastCheckTime;
    if (m_pressedDuration > m_longClickTime && !m_longPress ) {
      m_fsmState = PRESSED;	//long Event without release
      m_longPress = true;
    }

    if (!m_pressEdge && fn_press != nullptr && m_pressedDuration > m_bounceTime) {
      m_pressEdge = true;
      fn_press(this);
    }
  }
  else {
    m_pressEdge = false;
    if (fn_release != nullptr && m_pressedDuration > m_bounceTime) {
      fn_release(this);
    }

    if (m_pressedDuration > m_longClickTime) {
      m_fsmState = LONG_CLICK;	//long Event with release
      m_lastEventTime = current_millies;
      m_shortPressCnt = LONG_CLICK;
    }

    if (m_pressedDuration > m_bounceTime && m_pressedDuration <= m_longClickTime) {
      m_shortPressCnt++;
      m_lastEventTime = current_millies;
    }
    if (current_millies > m_lastEventTime + m_deadTime  && m_shortPressCnt > LONG_CLICK) {
      m_fsmState = m_shortPressCnt;
      m_shortPressCnt = LONG_CLICK;
    }

    m_longPress = false;
    m_pressedDuration = 0;
  }
  m_lastCheckTime = current_millies;
  return m_fsmState;
}

bool DigitalIn::click() {
  return m_fsmState == CLICK;
}

bool DigitalIn::doubleClick() {
  return m_fsmState >= N_CLICK;
}


bool DigitalIn::longClick(bool waitRelease) {
  if (waitRelease)
    return m_fsmState == LONG_CLICK;
  else
    return m_fsmState == PRESSED;
}