// 03 - Ultrasonic distance controls blink speed (with live calibration readout)
// HC-SR04: VCC -> 5V, GND -> GND, TRIG -> pin 10, ECHO -> pin 11
// LED: pin 9 -> 220ohm -> LED -> GND
//
// HOW TO CALIBRATE:
// Open the Serial Monitor (9600 baud). It prints the live distance in cm.
// Move your hand toward the sensor and note the closest and farthest
// readings you care about. Put those numbers into NEAR_CM and FAR_CM below,
// then re-upload. No separate calibration sketch needed.

const int TRIG = 10;                    // ping-out pin
const int ECHO = 11;                    // echo-in pin
const int LED  = 9;                     // LED pin

const int NEAR_CM = 5;                  // at/under this distance -> LED stays solid on
const int FAR_CM  = 40;                 // beyond this distance -> LED off

void setup() {                          // runs once at startup
  pinMode(TRIG, OUTPUT);                // TRIG sends out
  pinMode(ECHO, INPUT);                 // ECHO reads in
  pinMode(LED, OUTPUT);                 // LED pin sends out
  Serial.begin(9600);                   // open Serial Monitor at 9600
}

long readDistanceCM() {                 // helper: returns distance in cm (or -1 if nothing detected)
  digitalWrite(TRIG, LOW);              // trigger off to start clean
  delayMicroseconds(2);                 // tiny settle pause
  digitalWrite(TRIG, HIGH);             // start the ping
  delayMicroseconds(10);                // hold 10 microseconds
  digitalWrite(TRIG, LOW);              // stop the ping
  long duration = pulseIn(ECHO, HIGH, 30000); // time the echo (give up after 30ms)
  if (duration == 0) return -1;         // nothing came back
  return duration * 0.0343 / 2;         // convert time to centimeters
}

void loop() {                           // runs forever
  long cm = readDistanceCM();           // measure distance
  Serial.println(cm);                   // print it live -> this is your calibration readout

  if (cm < 0 || cm > FAR_CM) {          // nothing there, or too far away
    digitalWrite(LED, LOW);             // LED off
    return;                             // start the loop over
  }
  if (cm <= NEAR_CM) {                  // very close
    digitalWrite(LED, HIGH);            // LED solid on
    return;                             // done for this loop
  }
  // in between NEAR and FAR: closer = slower blink
  int blinkDelay = map(cm, NEAR_CM, FAR_CM, 600, 60); // turn distance into a blink delay in ms
  digitalWrite(LED, HIGH);              // LED on
  delay(blinkDelay);                    // wait (longer when closer)
  digitalWrite(LED, LOW);               // LED off
  delay(blinkDelay);                    // wait the same amount, then loop repeats
}
