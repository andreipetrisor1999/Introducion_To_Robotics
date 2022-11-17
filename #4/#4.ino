#define LONG_PRESS_TIME 2000
unsigned long pressedTime = 0;

const int joyXPin = A0;
const int joyYPin = A1;
const int joySWPin = 2;

const int joyLeftThreshold = 400;
const int joyRightThreshold = 600;
const int joyDownThreshold = 400;
const int joyUpThreshold = 600;
bool joyIsNeutral = true;
byte lastSWState = LOW;
int buttonPressCount = 0;

const int latchPin = 11; // STCP to 12 on Shift Register
const int clockPin = 10; // SHCP to 11 on Shift Register
const int dataPin = 12; // DS to 14 on Shift Register

const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

const byte regSize = 8;

int displayDigits[] = {
  segD1, segD2, segD3, segD4
};
int dpPosition = 0;
int displayedNumber = 10000;
const int displayCount = 4;

const int encodingsNumber = 16;

int encodingArray[encodingsNumber][regSize] = {
  {1, 1, 1, 1, 1, 1, 0, 0},  // 0
  {0, 1, 1, 0, 0, 0, 0, 0},  // 1
  {1, 1, 0, 1, 1, 0, 1, 0},  // 2
  {1, 1, 1, 1, 0, 0, 1, 0},  // 3
  {0, 1, 1, 0, 0, 1, 1, 0},  // 4
  {1, 0, 1, 1, 0, 1, 1, 0},  // 5
  {1, 0, 1, 1, 1, 1, 1, 0},  // 6
  {1, 1, 1, 0, 0, 0, 0, 0},  // 7
  {1, 1, 1, 1, 1, 1, 1, 0},  // 8
  {1, 1, 1, 1, 0, 1, 1, 0},  // 9
  {1, 1, 1, 0, 1, 1, 1, 0},  // A
  {0, 0, 1, 1, 1, 1, 1, 0},  // b
  {1, 0, 0, 1, 1, 1, 0, 0},  // C
  {0, 1, 1, 1, 1, 0, 1, 0},  // d
  {1, 0, 0, 1, 1, 1, 1, 0},  // E
  {1, 0, 0, 0, 1, 1, 1, 0},  // F
};

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode (joySWPin, INPUT_PULLUP);

  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
  Serial.begin(9600);
}

void loop() {
  byte swState = digitalRead(joySWPin);
  byte buttonIsOn = !swState;
  if(swState != lastSWState){
    if(buttonIsOn){ 
      pressedTime = millis(); 
      buttonPressCount ++;
      if(buttonPressCount == 2){
        buttonPressCount = 0;
      }
    }
    if(buttonPressCount == 1 && (millis() - pressedTime >= LONG_PRESS_TIME)){
      displayedNumber = 10000;
      dpPosition = 0;
      buttonPressCount = 0;
    }
    lastSWState = swState;
  }
  readAxisAndButton();
  writeNumber(displayedNumber, false);
  writeNumber(displayedNumber, true);
}

void activateDisplay(int displayNumber) { 
  // first, disable all the display digits
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  // then enable only the digit you want to use now
  digitalWrite(displayDigits[displayNumber], LOW);
}

void writeReg(int encoding[]) {
  digitalWrite(latchPin, LOW);
  for (int i = 0; i < regSize; i++) { // MSBFIRST 
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, encoding[i]);
    digitalWrite(clockPin, HIGH);
  }
  digitalWrite(latchPin, HIGH);
}

void writeNumber(int number, bool isDpOn) {
  int currentNumber = number; 
  int displayDigit = 0;
  int lastDigit = 0;
  while (currentNumber >= 9) {
    lastDigit = currentNumber % 10;
    if(isDpOn && dpPosition == displayDigit){
      encodingArray[lastDigit][regSize - 1] = 1;
    }
    else {
      encodingArray[lastDigit][regSize - 1] = 0;
    }
    writeReg(encodingArray[lastDigit]);
    activateDisplay(displayDigit);
    currentNumber /= 10;
    displayDigit++;
  }
}

void readAxisAndButton(){
  int joyX = analogRead(joyXPin);
  int joyY = analogRead(joyYPin);
  if(joyX < joyDownThreshold && joyIsNeutral){
    joyIsNeutral = false;
    if(buttonPressCount == 1){
      if((int)(displayedNumber / pow(10, dpPosition)) % 10 == 0){
        displayedNumber += 9 * pow(10, dpPosition);
        if(dpPosition == 3) displayedNumber ++;
      }else {
        displayedNumber -= pow(10, dpPosition);
      }
    }
  }

  if(joyX > joyUpThreshold && joyIsNeutral){
    joyIsNeutral = false;
    if(buttonPressCount == 1){
      if((int)(displayedNumber / pow(10, dpPosition)) % 10 == 9){
        displayedNumber -= 9 * pow(10, dpPosition);
      }else {
        displayedNumber += pow(10, dpPosition);
      }
    }
  }

  if(joyY < joyLeftThreshold && joyIsNeutral){
    joyIsNeutral = false;
    if(buttonPressCount == 0){
      if(dpPosition == displayCount-1){
        dpPosition = displayCount - 1;
      } else {
        dpPosition ++;      
      }
    }
  }

  if(joyY > joyRightThreshold && joyIsNeutral){
    joyIsNeutral = false;
    if(buttonPressCount == 0){
      if(dpPosition == 0){
        dpPosition = 0;
      } else {
        dpPosition --;
      }
    }
  }
   
  if(joyDownThreshold <= joyX && joyX <= joyUpThreshold && joyLeftThreshold <= joyY && joyY <= joyRightThreshold){
   joyIsNeutral = true;
  }
  Serial.println(displayedNumber);
}