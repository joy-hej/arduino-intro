// 02 - Blink only while a button is held
// LED: pin 9 -> 220ohm -> LED -> GND
// Button: connect one leg to pin 2, the other leg to GND (no resistor needed).
// We use the board's built-in pull-up resistor, so the pin reads HIGH when
// released and LOW when pressed.
// Watch the printed value in the Serial Monitor (9600 baud).

const int LED_PIN = 9;                  // friendly name for the LED pin
const int BTN_PIN = 2;                  // friendly name for the button pin

void setup() {                          // runs once at startup
  pinMode(LED_PIN, OUTPUT);             // LED pin sends voltage out
  pinMode(BTN_PIN, INPUT_PULLUP);       // button pin reads voltage in; turn on the internal pull-up
  Serial.begin(9600);                   // open the Serial Monitor at 9600 so we can see the button state
}

void loop() {                           // runs forever
  int pressed = digitalRead(BTN_PIN);   // read the button: HIGH when released, LOW when pressed
  Serial.println(pressed);              // print that value so you can watch it change

  if (pressed == LOW) {                 // LOW means the button IS being held down
    digitalWrite(LED_PIN, HIGH);        // LED on
    delay(250);                         // wait a quarter second
    digitalWrite(LED_PIN, LOW);         // LED off
    delay(250);                         // wait again -> this makes it blink while held
  } else {                              // otherwise the button is released
    digitalWrite(LED_PIN, LOW);         // keep the LED off
  }
}
