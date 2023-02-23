/*
Name: Saatvik
*/
int bluePin = 13;    // choose the pin for the Blue LED
int greenPin = 12;   // choose the pin for the Green LED
int redPin = 11;	 // choose the pin for the Red LED
int orangePin = 10;  // choose the pin for the Orange LED
int sensor1Pin = 3;  // choose the input pin (for PIR sensor 1)
int sensor2Pin = 2;  // choose the input pin (for PIR sensor 2) 
int sensor3Pin = 4;	 // choose the input pin (for PIR sensor 3)
// we start, assuming no motion detected for both sensors
int pirState1 = LOW;
int pirState2 = LOW;
int pirState3 = LOW;

void setup() {
// blue led will glow for sensor 1
pinMode(bluePin, OUTPUT); // declare LED as output
  
// green led will glow for sensor 2
pinMode(greenPin, OUTPUT);

// red led will glow for sensor 3
pinMode(redPin, OUTPUT);

pinMode(orangePin, OUTPUT);
  
pinMode(sensor1Pin, INPUT); // declare sensor 1 as input
pinMode(sensor2Pin, INPUT); // declare sensor 2 as input
pinMode(sensor3Pin, INPUT); // declare sensor 2 as input
 
// attach interrupt to input pin
attachInterrupt(digitalPinToInterrupt(sensor1Pin), motionDetected1, CHANGE);
attachInterrupt(digitalPinToInterrupt(sensor2Pin), motionDetected2, CHANGE);

pciSetup(sensor3Pin);
startTimer(0.5);
Serial.begin(9600);
}

void loop(){
// do nothing in loop function since we are using interrupt
}

void startTimer(double timerFrequency){
  noInterrupts();
  
  // calculate the value for OCR1A based on the timer frequency
  double prescalar= 1024.0;
  double cyclesPerSecond = 16000000.00 / prescalar;
  double cyclesPerInterrupt = cyclesPerSecond / timerFrequency;
  uint16_t ocrValue = (uint16_t) (cyclesPerInterrupt-1);
  
  // set the registers for timer 1
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A= ocrValue;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  
  interrupts();
}

ISR(TIMER1_COMPA_vect){
   digitalWrite(orangePin, digitalRead(orangePin) ^ 1);
}
void pciSetup(byte pin)
{
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

ISR (PCINT2_vect) // handle pin change interrupt for D0 to D7 here
 {
     motionDetected3();
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

// For Sensor 3
void motionDetected3() {
int val = digitalRead(sensor3Pin); // read input value

if (val == HIGH) { // check if the input is HIGH
digitalWrite(redPin, HIGH); // turn LED ON
if (pirState3 == LOW) {
// we have just turned on
Serial.println("Sensor3: Motion detected!");
// We only want to print on the output change, not state
pirState3 = HIGH;
}
} else {
digitalWrite(redPin, LOW); // turn LED OFF
if (pirState3 == HIGH){
// we have just turned off
Serial.println("Sensor3: Motion ended!");
// We only want to print on the output change, not state
pirState3 = LOW;
}
}
}