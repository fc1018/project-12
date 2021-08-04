#include <Arduino.h>
#include <Servo.h>

Servo myServo;
const int piezo = A0;
const int switchPin = 2;
const int yellowLED = 3;
const int greenLED = 4;
const int redLED = 5;
int knockVal;
int switchVal;
const int quietKnock = 10;
const int loudKnock = 100;
boolean locked = false;
int numberOfKnocks = 0;
boolean checkForKnock(int piezoVal, int knock_lowerbound, int knock_upperbound);

void setup() {
  myServo.attach(9);

  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(switchPin, INPUT);

  Serial.begin(9600);

  digitalWrite(greenLED, HIGH);
  myServo.write(0);
  Serial.println("The box is unlocked!");
}

void loop() {
  if (locked == false) {
    switchVal = digitalRead(switchPin);
    if (switchVal == HIGH) {
      locked = true;

      digitalWrite(greenLED, LOW);
      digitalWrite(redLED, HIGH);

      myServo.write(90);

      Serial.println("The box is locked!");

      delay(1000);
    }
  }
  if (locked == true) {
    knockVal = analogRead(piezo);
    if (numberOfKnocks < 3 && knockVal > 0) {
      if (checkForKnock(knockVal, quietKnock, loudKnock) == true) {
        numberOfKnocks++;
      }
      Serial.print(3 - numberOfKnocks);
      Serial.println("more knocks to go");
    }
    if (numberOfKnocks >= 3) {
      locked = false;
      myServo.write(0);
      delay(20);
      digitalWrite(greenLED, HIGH);
      digitalWrite(redLED, LOW);

      Serial.println("The box is unlocked!");
      numberOfKnocks = 0;
    }
  }
}

boolean checkForKnock(int piezoVal, int knock_lowerbound,
                      int knock_upperbound) {
  if (piezoVal > knock_lowerbound && piezoVal < knock_upperbound) {
    digitalWrite(yellowLED, HIGH);

    delay(50);

    digitalWrite(yellowLED, LOW);

    Serial.print("Valid knock of value\n");
    Serial.println(piezoVal);

    return true;
  } else {
    Serial.print("Bad knock value\n");
    Serial.println(piezoVal);

    return false;
  }
}
