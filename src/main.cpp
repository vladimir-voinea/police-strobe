#include "police_lights.hpp"
#include <Arduino.h>


void setup() {
  Serial.begin(9600);
}

void print_cycles(int cycles) {
  Serial.print("Cycles: ");
  Serial.println(cycles);
}

void loop() {
  static int cycles = 0;
  static police_lights lights;
  
  lights.loop();

  cycles++;
  //print_cycles(cycles);
}
