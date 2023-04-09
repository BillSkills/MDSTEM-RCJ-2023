#include <Arduino.h>
#include <SPI.h>
#include <Servo.h>
#include <algorithm>
#include <vector>

#define SERVO_PIN GPIO_NUM_13

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

void move() { SPI.transfer(0); }
void turnLeft() { SPI.transfer(1); }
void turnRight() { SPI.transfer(2); }
int ultrasonic() { return SPI.transfer(3); }
int ultrasonicLeft() { return SPI.transfer(4); }
int ultrasonicRight() { return SPI.transfer(5); }

// Setup
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);
  servo1.attach(SERVO_PIN);
  servo1.write(0);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  digitalWrite(SS, HIGH);
}

void loop() {
  int sensors[3];
  sensors[0] = ultrasonicLeft();
  sensors[1] = ultrasonic();
  sensors[2] = ultrasonicRight();

  for (int i = 0; i < 3; i++) {
    if (sensors[i] > 10) {
      sensors[i] = 0;
    } else {
      sensors[i] = 1;
    }
  }
}