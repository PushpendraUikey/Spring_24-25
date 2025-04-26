/*
  Arduino Starter Kit example
  Project 5 - Servo Mood Indicator

  This sketch is written to accompany Project 5 in the Arduino Starter Kit

  Parts required:
  - servo motor
  - 10 kilohm potentiometer
  - two 100 uF electrolytic capacitors

  created 13 Sep 2012
  by Scott Fitzgerald

  https://store.arduino.cc/genuino-starter-kit

  This example code is part of the public domain.
*/

// include the Servo library
#include <Servo.h>

Servo myServo;  // create a servo object


void setup() {
  myServo.attach(6);   // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);  // open a serial connection to your computer
  myServo.write(70);
  for(int i = 0 ; i < 20 ; i =i+1)
  {
    myServo.write(75);
    delay(40);
    myServo.write(65);
    delay(40);
  }
  for(int i = 70 ; i > 20 ; i =i-1)
  {
    myServo.write(i);
    delay(15);
  }
  delay(2000);
  for(int i = 20 ; i < 70 ; i =i+1)
  {
    myServo.write(i);
    delay(15);
  }
  
}

void loop() {
  
}
