#include <Arduino.h>
#include <MeMegaPi.h>
#include <SPI.h>
#include <Wire.h>
#include <vector>
using namespace std;

#define SPEED 200
#define MOVE_DISTANCE 30
#define WALL_DISTANCE 10

// Motor & sensor modules
MeMegaPiDCMotor motorLeft(PORT1B);
MeMegaPiDCMotor motorRight(PORT2B);
MeUltrasonicSensor ultrasonic(PORT_8);
MeColorSensor color(PORT_7);
MeRGBLed infoLight(PORT_6);
MeGyro gyro(PORT_5);

// Move function
void moveForward() {
  int intialDistance = ultrasonic.distanceCm();
  bool done = false;
  motorLeft.run(SPEED);
  motorRight.run(-SPEED);
  while (!done) {
    delay(10);
    done = intialDistance - MOVE_DISTANCE < ultrasonic.distanceCm();
  }
  motorLeft.stop();
  motorRight.stop();
}

// Left turn
void turnLeft() {
  int initialZ = gyro.getAngleZ();
  motorLeft.run(-50);
  motorRight.run(50);
  while (initialZ - 90 > gyro.getAngleZ()) {
    delay(10);
  }
  motorLeft.stop();
  motorRight.stop();
}

// Right turn
void turnRight() {
  int initialZ = gyro.getAngleZ();
  motorLeft.run(50);
  motorRight.run(-50);
  while (initialZ + 90 > gyro.getAngleZ()) {
    delay(10);
  }
  motorLeft.stop();
  motorRight.stop();
}

vector<int> tile(4);
vector<int> options;

// Init method
void setup() {
  infoLight.setColorAt(0, 255, 0, 0);
  infoLight.dWrite1(HIGH);

  // Sensor reading
  turnLeft();
  for (int i = 0; i < 4; i++) {
    tile[i] = ultrasonic.distanceCm();
    if (tile[i] > WALL_DISTANCE) {
      infoLight.setColorAt(0, 0, 255, 0);
      tile[i] = 0;
      options.push_back(i);
    } else {
      tile[i] = 1;
    }
    turnRight();
  }
  turnRight();
  delay(20);

  // Randomly selected direction to move in
  int moveChoice = rand() % options.size();
  switch (moveChoice) {
  case 0:
    turnLeft();
    break;
  case 1:
    break;
  case 2:
    turnRight();
    break;
  case 3:
    turnRight();
    turnRight();
    break;
  }

  infoLight.setColorAt(0, 255, 255, 255);
  tile.pop_back();
  delay(10);
}

// Main operation loop
void loop() {}

// OLD CODE FOR TALKING TO THE ESPs
/*
volatile bool received;
volatile byte byteReceived, byteSend;

// MeUltrasonicSensor ultrasonicLeft(PORT_7);
// MeUltrasonicSensor ultrasonicRight(PORT_6);

// Thing that runs when it receives an spi call?
ISR(SPI_STC_vect) {
  byteReceived = SPDR; // Store the incoming value
  received = true;     // yep
}

// Function for setting up the board as an SPI peripheral, so it can talk to the ESPs
int spi_init() {
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);

  SPCR |= _BV(SPE); // control register stuff i dont understand
                    // turns on spi in slave mode or something
  received = false;

  SPI.attachInterrupt();
}

void loop() {
  if (received) {
    switch (byteReceived) {
    case 0: // received 0, move 1 space
      move();
      break;
    case 1: // received 1, turn left
      turnLeft();
      break;
    case 2: // received 2, turn right
      turnRight();
      break;
    case 3: // received 3, return measurement from front ultrasonic
      byteSend = ultrasonic.distanceCm();
      break;
    case 4: // received 4, return measurement from front ultrasonic
      byteSend = ultrasonicLeft.distanceCm();
      break;
    case 5: // received 5, return measurement from front ultrasonic
      byteSend = ultrasonicRight.distanceCm();
      break;

    default:
      break;
    }

    received = false;
  }
  SPDR = byteSend; // sends data back to the master device
  delay(10);
}
*/