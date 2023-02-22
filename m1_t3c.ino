/*
Name: Saatvik
*/
int bluePin = 13;    // choose the pin for the Blue LED
int greenPin = 12;   // choose the pin for the Green LED
int sensor1Pin = 3;  // choose the input pin (for PIR sensor 1)
int sensor2Pin = 2;  // choose the input pin (for PIR sensor 2) 
 
// we start, assuming no motion detected for both sensors
int pirState1 = LOW;
int pirState2 = LOW;  

void setup() {
// blue led will glow for sensor 1
pinMode(bluePin, OUTPUT); // declare LED as output
  
// green led will glow for sensor 2
pinMode(greenPin, OUTPUT);
  
pinMode(sensor1Pin, INPUT); // declare sensor 1 as input
pinMode(sensor2Pin, INPUT); // declare sensor 2 as input
 
// attach interrupt to input pin
attachInterrupt(digitalPinToInterrupt(sensor1Pin), motionDetected1, CHANGE);
attachInterrupt(digitalPinToInterrupt(sensor2Pin), motionDetected2, CHANGE);

Serial.begin(9600);
}

void loop(){
// do nothing in loop function since we are using interrupt
}

// For Sensor 1
void motionDetected1() {
int val = digitalRead(sensor1Pin); // read input value

if (val == HIGH) { // check if the input is HIGH
digitalWrite(bluePin, HIGH); // turn LED ON
if (pirState1 == LOW) {
// we have just turned on
Serial.println("Sensor1: Motion detected!");
// We only want to print on the output change, not state
pirState1 = HIGH;
}
} else {
digitalWrite(bluePin, LOW); // turn LED OFF
if (pirState1 == HIGH){
// we have just turned off
Serial.println("Sensor1: Motion ended!");
// We only want to print on the output change, not state
pirState1 = LOW;
}
}
}

// For Sensor 2
void motionDetected2() {
int val = digitalRead(sensor2Pin); // read input value

if (val == HIGH) { // check if the input is HIGH
digitalWrite(greenPin, HIGH); // turn LED ON
if (pirState2 == LOW) {
// we have just turned on
Serial.println("Sensor2: Motion detected!");
// We only want to print on the output change, not state
pirState2 = HIGH;
}
} else {
digitalWrite(greenPin, LOW); // turn LED OFF
if (pirState2 == HIGH){
// we have just turned off
Serial.println("Sensor2: Motion ended!");
// We only want to print on the output change, not state
pirState2 = LOW;
}
}
}
