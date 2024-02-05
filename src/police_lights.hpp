#pragma once

#include <led.hpp>
#include <stddef.h>
#include <tuple>

enum class police_lights_state : char
{
    red,
    blue,
    off_1,
    sequence,
    off_2,
    all_on,
    off_3,
    STATE_COUNT
};

class police_lights
{
    using led = ardent::led;

  public:
    police_lights();
    void loop();
    void reset();

  private:
    int get_sleep_duration_ms();
    void sleep(int duration_ms);
    void transition_if_needed();

  private:
    int m_cycles = 0;
    int m_cycles_for_state = 0;
    int m_iterations = 0;
    police_lights_state m_state = police_lights_state::red;
    std::tuple<led, led, led> m_red;
    std::tuple<led, led, led> m_blue;
};