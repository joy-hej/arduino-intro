// 04 - Servo proximity sweep + button-held LED blink (running together)
// The servo sweeps back and forth whenever someone comes within range,
// AND the LED blinks whenever the button is held down. Both happen at the
// same time because we use millis() timing instead of delay() -- delay()
// would freeze everything else while it waits.
//
// WIRING:
// Servo signal (orange) -> pin 6
// Servo V+ (red)   -> external supply +   (POWER SERVO EXTERNALLY)
// Servo GND (brown)-> external supply -  AND -> Nano GND   (common ground)
// Button: one leg -> pin 2, other leg -> GND (uses internal pull-up)
// LED: pin 9 -> 220ohm -> LED -> GND
// HC-SR04: VCC -> 5V, GND -> GND, TRIG -> pin 10, ECHO -> pin 11

#include <Servo.h>                      // built-in library that controls servos

Servo myServo;                          // servo object we can command
const int SERVO_PIN = 6;                // servo signal pin
const int BTN_PIN   = 2;                // button pin
const int LED_PIN   = 9;                // LED pin
const int TRIG = 10;                    // ultrasonic ping-out pin
const int ECHO = 11;                    // ultrasonic echo-in pin

const int FAR_CM = 40;                  // sweep when something is closer than this many cm

const int SWEEP_STEP_MS = 10;           // how long between each 1-degree servo move (smaller = faster)
const int BLINK_MS      = 250;          // how long between LED on/off toggles

// --- servo state (we track these ourselves instead of using a for-loop) ---
int servoAngle = 90;                    // current servo angle
int servoDir = 1;                       // direction: +1 sweeping up, -1 sweeping down
bool sweeping = false;                  // is something in range right now?
unsigned long lastServoStep = 0;        // when we last moved the servo (in ms)

// --- LED blink state ---
bool ledOn = false;                     // is the LED currently on?
unsigned long lastBlinkToggle = 0;      // when we last flipped the LED (in ms)

// --- sensor timing (don't ping too often) ---
unsigned long lastPing = 0;             // when we last read the sensor (in ms)
const int PING_INTERVAL_MS = 60;        // how often to check distance
long lastCm = -1;                       // most recent distance reading

void setup() {                          // runs once at startup
  myServo.attach(SERVO_PIN);            // tell the library which pin the servo is on
  pinMode(BTN_PIN, INPUT_PULLUP);       // button reads in; internal pull-up means pressed = LOW
  pinMode(LED_PIN, OUTPUT);             // LED pin sends voltage out
  pinMode(TRIG, OUTPUT);                // TRIG sends out
  pinMode(ECHO, INPUT);                 // ECHO reads in
  myServo.write(servoAngle);            // start the servo at the middle
}

long readDistanceCM() {                 // helper: distance in cm, or -1 if nothing detected
  digitalWrite(TRIG, LOW);              // trigger off to start clean
  delayMicroseconds(2);                 // tiny settle pause (microseconds, so effectively instant)
  digitalWrite(TRIG, HIGH);             // start the ping
  delayMicroseconds(10);                // hold 10 microseconds
  digitalWrite(TRIG, LOW);              // stop the ping
  long duration = pulseIn(ECHO, HIGH, 30000); // time the echo (give up after 30ms)
  if (duration == 0) return -1;         // nothing came back
  return duration * 0.0343 / 2;         // convert time to centimeters
}

void loop() {                           // runs forever, very fast, without ever blocking
  unsigned long now = millis();         // millis() = how many ms since the board turned on

  // 1) Check the sensor every PING_INTERVAL_MS
  if (now - lastPing >= PING_INTERVAL_MS) {
    lastPing = now;                     // remember this ping time
    lastCm = readDistanceCM();          // measure distance
    sweeping = (lastCm > 0 && lastCm < FAR_CM); // sweep only if something is in range
  }

  // 2) Move the servo one step if it's time and we're sweeping
  if (sweeping) {
    if (now - lastServoStep >= SWEEP_STEP_MS) {
      lastServoStep = now;              // remember this step time
      servoAngle += servoDir;           // move one degree in the current direction
      if (servoAngle >= 180) { servoAngle = 180; servoDir = -1; } // hit the top -> reverse
      if (servoAngle <= 0)   { servoAngle = 0;   servoDir = 1;  } // hit the bottom -> reverse
      myServo.write(servoAngle);        // send the servo to the new angle
    }
  } else {                              // nothing in range
    myServo.write(90);                  // rest at the middle
    servoAngle = 90;                    // reset our tracking
    servoDir = 1;
  }

  // 3) Blink the LED while the button is held (independent of the servo)
  if (digitalRead(BTN_PIN) == LOW) {    // LOW means the button is pressed
    if (now - lastBlinkToggle >= BLINK_MS) {
      lastBlinkToggle = now;            // remember this toggle time
      ledOn = !ledOn;                   // flip the LED state
      digitalWrite(LED_PIN, ledOn ? HIGH : LOW); // apply it
    }
  } else {                              // button released
    ledOn = false;                      // reset state
    digitalWrite(LED_PIN, LOW);         // LED off
  }
}
