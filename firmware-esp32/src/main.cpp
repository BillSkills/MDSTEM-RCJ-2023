/*
main.cpp (ESP32)
April 12, 2023
MDSTEM-RCJ-2023
Usan Siriwardana, William Zheng
*/

#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>
#include <algorithm>

#define SERVO_PIN GPIO_NUM_2

Servo servo1;

int rescueKits = 12;

// Deploy 1 rescue kit
void deployKit() {
  if (rescueKits > 0) {
    servo1.write(180);
    delay(100);
    servo1.write(0);
    rescueKits--;
  }
}

// Deploy multiple rescue kits
void deployKit(int numKits) {
  for (int i = 0; i < numKits; i++) {
    deployKit();
  }
}

bool move() { return SPI.transfer(0); }
void turnLeft() { SPI.transfer(1); }
void turnRight() { SPI.transfer(2); }
bool wallDetect() { return SPI.transfer(3); }

// Setup
void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);
  servo1.attach(SERVO_PIN);
  servo1.write(0);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  digitalWrite(SS, HIGH);

  move();
  turnLeft();
  turnRight();
}

bool open[3]; // Stores the 3 directions, left, front, right
int randInt = random(3);
void loop() {


  // // scanning phase
  // turnLeft();
  // for (int i = 0; i < 3; i++) {
  //   open[i] = wallDetect();
  //   turnRight();
  // }

  // // pick an open side
  // while (!open[randInt]) {
  //   randInt = random(3);
  // }

  // // move through one of the open slots
  // switch (randInt) {
  // case 0:
  //   turnLeft();
  //   turnLeft();
  //   move();
  //   break;
  // case 1:
  //   turnLeft();
  //   move();
  //   break;
  // case 2:
  //   move();
  //   break;
  // default:
  //   break;
  // }
}