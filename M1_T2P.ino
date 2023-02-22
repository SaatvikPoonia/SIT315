/*
PIR sensor tester with interrupt
*/
int ledPin = 12; // choose the pin for the LED
int inputPin = 2; // choose the input pin (for PIR sensor)
int pirState = LOW; // we start, assuming no motion detected

void setup() {
pinMode(ledPin, OUTPUT); // declare LED as output
pinMode(inputPin, INPUT); // declare sensor as input
attachInterrupt(digitalPinToInterrupt(inputPin), motionDetected, CHANGE); // attach interrupt to input pin

Serial.begin(9600);
}

void loop(){
// do nothing in loop function since we are using interrupt
}

void motionDetected() {
int val = digitalRead(inputPin); // read input value

if (val == HIGH) { // check if the input is HIGH
digitalWrite(ledPin, HIGH); // turn LED ON
if (pirState == LOW) {
// we have just turned on
Serial.println("Motion detected!");
// We only want to print on the output change, not state
pirState = HIGH;
}
} else {
digitalWrite(ledPin, LOW); // turn LED OFF
if (pirState == HIGH){
// we have just turned off
Serial.println("Motion ended!");
// We only want to print on the output change, not state
pirState = LOW;
}
}
}