#include "police_lights.hpp"
#include <push_button.hpp>
#include <time.hpp>
#include <sleep.hpp>
#include <memory>
#include <Arduino.h>

enum class strobe_mode
{
  off,
  on
} lights_state = strobe_mode::off;

void setup() {
  Serial.begin(9600);
}

void print_cycles(int cycles) {
  Serial.print("Cycles: ");
  Serial.println(cycles);
}

void loop() {
  static int cycles = 0;
  static auto lights = vl::make_unique<police_lights>();
  static ardent::push_button button{8};
  
  if(lights_state == strobe_mode::on) {
    lights->loop();
  }

  if(button.is_pressed()) {
    lights_state = lights_state == strobe_mode::on ? strobe_mode::off : strobe_mode::on;
    lights->reset();

    ardent::sleep(vl::milliseconds{500});    
  }

  cycles++;
  //print_cycles(cycles);
}
