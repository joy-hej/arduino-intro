// 00 - Hello Serial
// First real upload: prints a message once per second.
// Open the Serial Monitor and set the baud dropdown to 9600.

void setup() {                          // runs once when the board powers on or resets
  Serial.begin(9600);                   // open the serial connection at 9600 bits/sec so we can print text
}

void loop() {                           // runs over and over, forever
  Serial.println("Hello, sculpture!");  // send this line of text to the Serial Monitor
  delay(1000);                          // wait 1000 milliseconds (1 second) before repeating
}
