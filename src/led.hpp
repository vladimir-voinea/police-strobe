#pragma once

class led
{
public:
  led(int pin_number);
  void turn_on();
  void turn_off();

private:
  int m_pin_number;
};