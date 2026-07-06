// 01 - Blink an LED
// Pin 9 -> 220ohm resistor -> LED anode (long leg) -> cathode -> GND
// Change the delay() values to change the blink speed.

const int LED_PIN = 9;                  // give pin 9 a friendly name so the code is easy to read

void setup() {                          // runs once at startup
  pinMode(LED_PIN, OUTPUT);             // set pin 9 as an output so it can push voltage to the LED
}

void loop() {                           // runs forever
  digitalWrite(LED_PIN, HIGH);          // turn the pin ON (~5V) -> LED lights up
  delay(500);                           // keep it on for 500 ms (half a second)
  digitalWrite(LED_PIN, LOW);           // turn the pin OFF (0V) -> LED goes dark
  delay(500);                           // keep it off for 500 ms, then the loop repeats
}
