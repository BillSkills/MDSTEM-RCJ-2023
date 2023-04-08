#include <Arduino.h>
#include <MeMegaPi.h>
#include <Wire.h>

#define SPEED 200

MeMegaPiDCMotor motorL(PORT1B);
MeMegaPiDCMotor motorR(PORT2B);
MeUltrasonicSensor ultrasonic(PORT_8);
MeGyro gyro(PORT6);

void move(int distance) {
  int intialDistance = ultrasonic.distanceCm();
  bool done = false;
  motorL.run(SPEED);
  motorR.run(-SPEED);
  while (!done) {
    delay(10);
    done = intialDistance - distance < ultrasonic.distanceCm();
  }

  // delay(distance*100);

  motorL.stop();
  motorR.stop();
}

void turnRight() {
  int initialZ = gyro.getAngleZ();
  motorL.run(50);
  motorR.run(-50);
  while (initialZ + 90 > gyro.getAngleZ()) {
    delay(10);
  }
  motorL.stop();
  motorR.stop();
}

void turnLeft() {
  int initialZ = gyro.getAngleZ();
  motorL.run(-50);
  motorR.run(50);
  while (initialZ - 90 > gyro.getAngleZ()) {
    delay(10);
  }
  motorL.stop();
  motorR.stop();
}

void setup() {
  Serial.begin(9600);
  move(30);
}

void loop() {
  Serial.print("Distance: ");
  Serial.print(ultrasonic.distanceCm());
  Serial.println(" cm");
  delay(100);
}