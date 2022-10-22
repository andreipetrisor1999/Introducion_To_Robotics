//RGB LED constants - common Cathode
const int redLedPin = 11;
const int greenLedPin = 10;
const int blueLedPin = 9;
const int minLedBrightness = 0;
const int maxLedBrightness = 255;

//potentiometer constants
const int redPotPin = A5;
const int greenPotPin = A4;
const int bluePotPin = A3;
const int minVoltageValue = 0;
const int maxVoltageValue = 1023;

void setup() {
  // RGB LED setup
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);

  //potentiometer setup
  pinMode(redPotPin, INPUT);
  pinMode(greenPotPin, INPUT);
  pinMode(bluePotPin, INPUT);
}

void loop() {
  //reading potentiometers
  int redPotValue = analogRead(redPotPin);
  int greenPotValue = analogRead(greenPotPin);  
  int bluePotValue = analogRead(bluePotPin);

  //mapping potentiometer values for all collors
  long redColorValue = map(redPotValue, minVoltageValue, maxVoltageValue, minLedBrightness, maxLedBrightness);
  long greenColorValue = map(greenPotValue, minVoltageValue, maxVoltageValue, minLedBrightness, maxLedBrightness);
  long blueColorValue = map(bluePotValue, minVoltageValue, maxVoltageValue, minLedBrightness, maxLedBrightness);

  //setting color to RGB LED
  setColor(redColorValue, greenColorValue, blueColorValue);
}

void setColor(int redColorValue, int greenColorValue, int blueColorValue){
    analogWrite(redLedPin, redColorValue);
    analogWrite(greenLedPin, greenColorValue);
    analogWrite(blueLedPin, blueColorValue);
}