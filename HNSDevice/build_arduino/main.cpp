#include "../lib/HNSArduino/Arduino.h"
#include <util/delay.h>
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  _delay_ms(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  _delay_ms(1000);                       // wait for a second
}

int main(){
  setup();
  while(1){
    loop();
  }
}
