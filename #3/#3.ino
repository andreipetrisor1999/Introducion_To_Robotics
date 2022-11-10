#define BLINK_INTERVAL 400
#define LONG_PRESS_TIME 1000
unsigned long pressedTime = 0;
unsigned long previousBlinkTime = 0;
int buttonPressCount = 0;
bool changingSegmentValue = false;
byte lastSWState = LOW;

const int joyXPin = A0;
const int joyYPin = A1;
const int joySWPin = 2;

const int joyLeftThreshold = 400;
const int joyRightThreshold = 600;
const int joyDownThreshold = 400;
const int joyUpThreshold = 600;
bool joyIsNeutral = true;
bool segmentMoved = false;

const int pinA = 4;
const int pinB = 5;
const int pinC = 6;
const int pinD = 7;
const int pinE = 8;
const int pinF = 9;
const int pinG = 10;
const int pinDP = 11;

const int segSize =8;
byte currentSegment[segSize] = {0, 0, 0, 0, 0, 0, 0, 1};
byte segmentsOn[segSize] = {0, 0, 0, 0, 0, 0, 0, 1};
byte segmentsOff[segSize] = {0, 0, 0, 0, 0, 0, 0, 0};

int segments[segSize] = {pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP};
byte segmentON = HIGH;

void setup() {
  for(int i = 0; i < segSize; i++) {
    pinMode (segments[i], OUTPUT);
  }
  pinMode (joySWPin, INPUT_PULLUP);
}

void loop() {
  if(changingSegmentValue == false) selectSegment();
  byte swState = digitalRead(joySWPin);
  byte buttonIsOn = !swState;
  int joyX2 = analogRead(joyXPin);
  if(swState != lastSWState){
    if(buttonIsOn){  
      pressedTime = millis();  
      changingSegmentValue = true;
      buttonPressCount ++;
      if(buttonPressCount == 1){
        for(int i = 0; i < segSize; i++){
          if(currentSegment[i] == 1 && segmentsOff[i] == 0){
            segmentsOff[i] = 1;           
          }
          else {
            if(currentSegment[i] == 1 && segmentsOff[i] == 1){
              segmentsOff[i] = 0;
            }
          }
          if(joyX2 < joyDownThreshold && joyIsNeutral){
            joyIsNeutral = false;
            segmentsOff[i] = 1 ? segmentsOff[i] = 0 : segmentsOff[i] = 1;
            digitalWrite(segments[i], segmentsOff[i]);
          }
          if(joyX2 > joyUpThreshold && joyIsNeutral){
            joyIsNeutral = false;
            segmentsOff[i] = 1 ? segmentsOff[i] = 0 : segmentsOff[i] = 1;
            digitalWrite(segments[i], segmentsOff[i]);
          } 
          if(joyDownThreshold <= joyX2 && joyX2 <= joyUpThreshold){
            joyIsNeutral = true;
          }   
          digitalWrite(segments[i], segmentsOff[i]); 
        }
      }
      else {
        if(buttonPressCount == 2){
          buttonPressCount = 0;
          changingSegmentValue = false;
        }
      }
    }
    if(millis() - pressedTime >= LONG_PRESS_TIME){
        changingSegmentValue = false;
        buttonPressCount = 0;
        memset(currentSegment, 0, sizeof(currentSegment));
        memset(segmentsOn, 0, sizeof(segmentsOn));
        memset(segmentsOff, 0, sizeof(segmentsOff));
        currentSegment[segSize - 1] = 1;
        segmentsOn[segSize - 1] = 1;   
      }
    lastSWState = swState;
  }

  if( (millis() - previousBlinkTime >= BLINK_INTERVAL) && changingSegmentValue == false) {
    previousBlinkTime = millis();
    if(segmentON == HIGH) {
      for(int i = 0; i < segSize; i++){
        segmentsOn[i] = segmentsOff[i];
        if(currentSegment[i] == 1 && segmentsOff[i] == 1) {
          segmentsOn[i] = 0;
        }
        if(currentSegment[i] == 1 && segmentsOff[i] == 0) {
          segmentsOn[i] = 1;
        }
        digitalWrite(segments[i], segmentsOn[i]);
      }
      segmentON = LOW; 
    } 
    else {
      for(int i = 0; i < segSize; i++){
        digitalWrite(segments[i], segmentsOff[i]);
      }
      segmentON = HIGH;
    }
  }
}

void selectSegment(){
  int joyY = analogRead(joyYPin);
  int joyX = analogRead(joyXPin);
  if(joyY < joyLeftThreshold && joyIsNeutral){
    joyIsNeutral = false;
    moveLeft();
  }

  if(joyY > joyRightThreshold && joyIsNeutral){
    joyIsNeutral = false;
    moveRight();
  }
  if(joyX < joyDownThreshold && joyIsNeutral){
    joyIsNeutral = false;
    moveDown();
  }

  if(joyX > joyUpThreshold && joyIsNeutral){
    joyIsNeutral = false;
    moveUp();
  } 
  if(joyDownThreshold <= joyX && joyX <= joyUpThreshold && joyLeftThreshold <= joyY && joyY <= joyRightThreshold){
   joyIsNeutral = true;
   segmentMoved = false;
  }
}

void moveLeft(){
 if(currentSegment[pinDP-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinDP-pinA] = 0;
    currentSegment[pinE-pinA] = 1;
    segmentMoved = true;
  }
  if(currentSegment[pinD-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinD-pinA] = 0;
    currentSegment[pinE-pinA] = 1;
    segmentMoved = true;
  }
  if(currentSegment[pinC-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinC-pinA] = 0;
    currentSegment[pinE-pinA] = 1;
    segmentMoved = true;
  }
  if(currentSegment[pinB-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinB-pinA] = 0;
    currentSegment[pinF-pinA] = 1;
    segmentMoved = true;
  }
  if(currentSegment[pinA-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinA-pinA] = 0;
    currentSegment[pinF-pinA] = 1;
    segmentMoved = true;
  }
}

void moveRight(){
  if(currentSegment[pinD-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinD-pinA] = 0;
    currentSegment[pinC-pinA] = 1;
    segmentMoved = true;
  }
  if(currentSegment[pinE-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinE-pinA] = 0;
    currentSegment[pinC-pinA] = 1;
    segmentMoved = true;
  }
  if(currentSegment[pinC-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinC-pinA] = 0;
    currentSegment[pinDP-pinA] = 1;
    segmentMoved = true;
  }
  if(currentSegment[pinF-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinF-pinA] = 0;
    currentSegment[pinB-pinA] = 1;
    segmentMoved = true;
  }
  if(currentSegment[pinA-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinA-pinA] = 0;
    currentSegment[pinB-pinA] = 1;
    segmentMoved = true;
 }
}

void moveUp(){
  if(currentSegment[pinC-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinC-pinA] = 0;
    currentSegment[pinG-pinA] = 1;
    segmentMoved = true;
  }
  if(currentSegment[pinB-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinB-pinA] = 0;
    currentSegment[pinA-pinA] = 1;
    segmentMoved = true;
  }
  if(currentSegment[pinD-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinD-pinA] = 0;
    currentSegment[pinG-pinA] = 1;
    segmentMoved = true;
  }
  if(currentSegment[pinG-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinG-pinA] = 0;
    currentSegment[pinA-pinA] = 1;
    segmentMoved = true;
  }
  if(currentSegment[pinE-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinE-pinA] = 0;
    currentSegment[pinG-pinA] = 1;
    segmentMoved = true;
 }
  if(currentSegment[pinF-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinF-pinA] = 0;
    currentSegment[pinA-pinA] = 1;
    segmentMoved = true;
 }
}

void moveDown(){
  if(currentSegment[pinA-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinA-pinA] = 0;
    currentSegment[pinG-pinA] = 1;
    segmentMoved = true;
  }
  if(currentSegment[pinF-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinF-pinA] = 0;
    currentSegment[pinG-pinA] = 1;
    segmentMoved = true;
  }
  if(currentSegment[pinB-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinB-pinA] = 0;
    currentSegment[pinG-pinA] = 1;
    segmentMoved = true;
  }
  if(currentSegment[pinG-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinG-pinA] = 0;
    currentSegment[pinD-pinA] = 1;
    segmentMoved = true;
  }
  if(currentSegment[pinE-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinE-pinA] = 0;
    currentSegment[pinD-pinA] = 1;
    segmentMoved = true;
  }
  if(currentSegment[pinC-pinA] == 1 && joyIsNeutral == false && segmentMoved == false) {
    currentSegment[pinC-pinA] = 0;
    currentSegment[pinD-pinA] = 1;
    segmentMoved = true;
 }
}