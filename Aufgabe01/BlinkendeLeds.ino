// LED - test : Blinking with 1Hz frequency
// LED is connected to Pin 10

// Global Variables
int ledPin = 10;
int led2Pin = 11;

void setup(){
  // Initialize Output Pins
  pinMode(ledPin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
}

void loop(){
  // enable the LED
  digitalWrite(ledPin, HIGH);
  digitalWrite(led2Pin, LOW);
  // wait for 500ms
  delay(500);
  // disable the LED
  digitalWrite(ledPin, LOW);
  digitalWrite(led2Pin, HIGH);
  // wait for 500ms
  delay(500);
}
