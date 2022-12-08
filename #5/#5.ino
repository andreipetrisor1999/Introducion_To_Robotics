#include <LiquidCrystal.h>
#include <EEPROM.h>
#define MAIN_MENU_SIZE 5
#define HIGHSCORE_MENU_SIZE 5

const int joyXPin = A0;
const int joyYPin = A1;
const int joySWPin = 2;
const int joyLeftThreshold = 400;
const int joyRightThreshold = 600;
const int joyDownThreshold = 400;
const int joyUpThreshold = 600;
bool joyIsNeutral = true;
byte lastSwState = LOW;
 
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
 
const byte lcdContrastPin = 3;
 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

enum menuTypes {
  MAIN,
  HIGH_SCORES,
  SETTINGS,
  ABOUT,
  HOW_TO_PLAY
};
menuTypes currentMenu = MAIN;
bool accesedSubMenu = false;

String mainMenuItems[MAIN_MENU_SIZE] = {" Start Game", " High Scores ", " Settings", " About", " How to play"};
String highScoresMenuItems[HIGHSCORE_MENU_SIZE];
int cursorPosition = 0;
char cursor = '>';


byte lcdContrast;

void setup() {
  Serial.begin(9600);
  pinMode(lcdContrastPin, OUTPUT);
  pinMode (joySWPin, INPUT_PULLUP);
  lcd.begin(16, 2);
  lcdContrast = EEPROM.read(0);
  analogWrite(lcdContrastPin, lcdContrast);
}

void loop() {
  Serial.print(cursorPosition);
  Serial.print(",");
  Serial.print(currentMenu);
  Serial.println();
  if(millis() < 1000){
    lcd.print(" THE COOL GAME ");
    lcd.setCursor(0, 1);
    lcd.print(" welcome player ");
  }else {
    if(millis() < 1200) {
      moveMainMenu(cursorPosition);
    } 
    else {
        readAxisAndButton(currentMenu, MAIN_MENU_SIZE);
    }
  }
}

void readAxisAndButton(enum menuTypes menuType, int currentMenuSize){
  byte swState = digitalRead(joySWPin);
  Serial.print(swState);
  Serial.println();
  int joyX = analogRead(joyXPin);
  int joyY = analogRead(joyYPin);
  if(joyX < joyDownThreshold && joyIsNeutral && cursorPosition < currentMenuSize - 1){
    joyIsNeutral = false;
    cursorPosition ++;
    switch (menuType){
      case MAIN:
        if(cursorPosition != currentMenuSize - 1){
          moveMainMenu(cursorPosition);
        } else {
            for(int i=0; i<currentMenuSize; i++){
              mainMenuItems[i].setCharAt(0,' ');
            }
            lcd.clear();
            mainMenuItems[cursorPosition].setCharAt(0,cursor);
            lcd.print(mainMenuItems[cursorPosition-1]);
            lcd.setCursor(0, 1);
            lcd.print(mainMenuItems[cursorPosition]);
          }
        break;
      case HIGH_SCORES:
        if(highScoresMenuItems[0].length() == 0){
          lcd.clear();
          lcd.print(" NO HIGH SCORES ");
          lcd.setCursor(0, 1);
          lcd.print("   AVAILABLE   ");
        }
        break;
    } 
  }

  if(joyX > joyUpThreshold && joyIsNeutral && cursorPosition > 0){
    joyIsNeutral = false;
    cursorPosition --;
    switch (menuType){
      case MAIN:
          moveMainMenu(cursorPosition);
          break;
      case HIGH_SCORES:
          if(highScoresMenuItems[0].length() == 0){
            lcd.clear();
            lcd.print(" NO HIGH SCORES ");
            lcd.setCursor(0, 1);
            lcd.print("   AVAILABLE   ");
          }
          break;
    }
  }

  if(joyY < joyLeftThreshold && joyIsNeutral){
    joyIsNeutral = false;
  }

  if(joyY > joyRightThreshold && joyIsNeutral){
    joyIsNeutral = false;
  }
   
  if(joyDownThreshold <= joyX && joyX <= joyUpThreshold && joyLeftThreshold <= joyY && joyY <= joyRightThreshold){
   joyIsNeutral = true;
  }

  if(swState != lastSwState){
    if(swState == LOW){
      if(accesedSubMenu == false){
        switch(cursorPosition){
          case 1:
            accesedSubMenu = true;
            currentMenu = HIGH_SCORES;
            if(highScoresMenuItems[0].length() == 0){
            lcd.clear();
            lcd.print(" NO HIGH SCORES ");
            lcd.setCursor(0, 1);
            lcd.print("   AVAILABLE   ");
          }
        }
      } else {
          accesedSubMenu = false;
          currentMenu = MAIN;
          moveMainMenu(0);        
      }
    }
    lastSwState = swState;
  }
}

void moveMainMenu(int position){
  for(int i=0; i<MAIN_MENU_SIZE; i++){
    mainMenuItems[i].setCharAt(0,' ');
  }
  lcd.clear();
  mainMenuItems[position].setCharAt(0,cursor);
  lcd.print(mainMenuItems[position]);
  lcd.setCursor(0, 1);
  lcd.print(mainMenuItems[position+1]);
}