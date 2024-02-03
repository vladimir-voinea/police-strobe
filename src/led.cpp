#include "led.hpp"
#include "Arduino.h"

led::led(int pin_number)
  : m_pin_number(pin_number)
{
  pinMode(m_pin_number, OUTPUT);
}

void led::turn_on()
{
  digitalWrite(m_pin_number, HIGH);
}

void led::turn_off()
{
  digitalWrite(m_pin_number, LOW);
}