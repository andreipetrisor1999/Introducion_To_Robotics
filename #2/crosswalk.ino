const int buzzerPin = 2;

//Traffic Lights
const int trfGreenLight = 4;
const int trfYellowLight = 5;
const int trfRedLight = 6;

// Pedestrian lights
const int pedRedtLightPin = 8;
const int pedGreenLightPin = 9;

const int buttonPin = 11;
byte lastButtonState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(trfGreenLight, OUTPUT);
  pinMode(trfYellowLight, OUTPUT);
  pinMode(trfRedLight, OUTPUT);

  pinMode(pedRedtLightPin, OUTPUT);
  pinMode(pedGreenLightPin, OUTPUT);

  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(buzzerPin, OUTPUT);
  stateOne ();
}

void loop() {
  byte buttonState = digitalRead(buttonPin);
  if(buttonState != lastButtonState){
    if(buttonState == LOW) {
        delay(8000);
        stateTwo();
        stateThree();
        stateFour();
        stateOne();
    }
    lastButtonState = buttonState;
  }
}

void stateOne (){
  digitalWrite (trfGreenLight, HIGH);
  digitalWrite (trfYellowLight, LOW);
  digitalWrite (trfRedLight, LOW);

  digitalWrite (pedRedtLightPin, HIGH);
  digitalWrite (pedGreenLightPin, LOW);
  noTone (buzzerPin);
}

void stateTwo (){
  digitalWrite (trfGreenLight, LOW);
  digitalWrite (trfYellowLight, HIGH);
  
  delay(3000);
}

void stateThree (){
  digitalWrite (trfYellowLight, LOW);
  digitalWrite (trfRedLight, HIGH);

  digitalWrite (pedRedtLightPin, LOW);
  digitalWrite (pedGreenLightPin, HIGH);
  for (int i=0; i <= 4; i++) {
  tone(buzzerPin, 1000);
  delay(1000);
  noTone(buzzerPin);
  delay(1000);
  }
}

void stateFour (){
  for(int i = 0; i <= 4000; i += 800){
    digitalWrite(pedGreenLightPin, LOW);
    tone(buzzerPin, 1200);
    delay(400);
    digitalWrite(pedGreenLightPin, HIGH);
    noTone(buzzerPin);
    delay(400);
  }
}

