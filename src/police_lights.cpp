#include "police_lights.hpp"
#include "Arduino.h"

namespace
{
    constexpr auto base_duration = 50;

    [[maybe_unused]] const char* to_string(police_lights_state state)
    {
        switch(state)
        {
            case police_lights_state::red:
                return "red";
            case police_lights_state::blue:
                return "blue";
            case police_lights_state::sequence:
                return "sequence";
            case police_lights_state::off_1:
                return "off_1";
            case police_lights_state::off_2:
                return "off_2";
            default:
                break;
        }

        return "unknown";
    }
}

using ardent::led;

police_lights::police_lights()
    : m_red({2}, {3}, {4}), m_blue({5}, {6}, {7})
{
}

void police_lights::reset()
{
    m_cycles = 0;
    m_cycles_for_state = 0;
    m_iterations = 0;
    m_state = police_lights_state::red;
}

void police_lights::transition_if_needed()
{
    if(!m_cycles)
    {
        return;
    }

    static auto go_to_next_state = [this, state_changes = 0] () mutable {
        m_state = static_cast<police_lights_state>((static_cast<int>(m_state) + 1) % static_cast<char>(police_lights_state::STATE_COUNT));
        m_cycles_for_state = 0;
        state_changes++;

        if(state_changes % static_cast<char>(police_lights_state::STATE_COUNT) == 0)
        {
            m_iterations++;
        }

        // Serial.print("State changes: ");
        // Serial.println(state_changes);
        // Serial.print("Iterations: ");
        // Serial.println(m_iterations);
    }; 

    switch(m_state)
    {
        case police_lights_state::red:
            if(m_cycles_for_state == 10)
            go_to_next_state();
            break;
        case police_lights_state::blue:
            if(m_cycles_for_state == 10)
            go_to_next_state();
            break;
        case police_lights_state::off_1:
            if(m_cycles_for_state == 5)
            go_to_next_state();
            break;
        case police_lights_state::sequence:
            if(m_cycles_for_state == 10)
            go_to_next_state();
            break;
        case police_lights_state::off_2:
            if(m_cycles_for_state == 5)
            go_to_next_state();
            break;
        case police_lights_state::all_on:
            if(m_cycles_for_state == 10)
            go_to_next_state();
            break;
        case police_lights_state::off_3:
            if(m_cycles_for_state == 5)
            go_to_next_state();
            break;
        default:
            break;
    }
}

struct blink
{
    blink(int duration_ms)
        : duration_ms(duration_ms)
    {}

    void operator()(led& l) const
    {
        l.turn_on();
        // Serial.print("Sleeping ");
        // Serial.print(duration_ms);
        // Serial.println("ms");
        delay(duration_ms);
        l.turn_off();
    }
    
    int duration_ms;
};

template<typename T>
void blink_all(int duration_ms, T& leds)
{
    auto turn_on = [] (led& led) {
        led.turn_on();
    };

    auto turn_off = [] (led& led) {
        led.turn_off();
    };
    
    estd::apply_to_each(turn_on, leds);
    delay(duration_ms);
    estd::apply_to_each(turn_off, leds);
    delay(duration_ms);
}

void police_lights::loop()
{
    transition_if_needed();
    
    [[maybe_unused]]
    auto turn_on = [] (led& led) {
        led.turn_on();
    };

    auto turn_off = [] (led& led) {
        led.turn_off();
    };

    switch(m_state)
    {
        case police_lights_state::red:
            estd::apply_to_each(turn_off, m_blue);
            estd::apply_to_each(blink{get_sleep_duration_ms()}, m_red);
            break;
        case police_lights_state::blue:
            estd::apply_to_each(turn_off, m_red);
            estd::apply_to_each(blink{get_sleep_duration_ms()}, m_blue);
            break;
        case police_lights_state::sequence:
            estd::apply_to_each(blink{static_cast<int>(get_sleep_duration_ms() / 1.25f)}, m_red);
            estd::apply_to_each(blink{static_cast<int>(get_sleep_duration_ms() / 1.25f)}, m_blue);
            break;
        case police_lights_state::all_on:
        {
            constexpr auto blinks = 3;
            for(int i = 0; i < blinks; i++)
            {
                blink_all(get_sleep_duration_ms() * 1.5f, m_red);
            }

            for(int i = 0; i < blinks; i++)
            {
                blink_all(get_sleep_duration_ms() * 1.5f, m_blue);
            }
            break;
        }
        case police_lights_state::off_1:
        case police_lights_state::off_2:
        case police_lights_state::off_3:
            estd::apply_to_each(turn_off, m_red);
            estd::apply_to_each(turn_off, m_blue);
            sleep(get_sleep_duration_ms());
            break;
        default:
            break;
    }

    m_cycles++;
    m_cycles_for_state++;
}

void police_lights::sleep(int duration_ms)
{
    delay(duration_ms);
}

int police_lights::get_sleep_duration_ms()
{
    const auto multiplier = m_iterations % 2 == 0 ? 1.f : 1.7f;
    return static_cast<int>(base_duration * multiplier);
}