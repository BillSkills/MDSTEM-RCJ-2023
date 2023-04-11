#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <MeMegaPi.h>

#define SPEED 200
#define MOVE_DISTANCE 30
#define WALL_DISTANCE 10
#define TERRAIN_WAIT 5000

volatile bool received;
volatile byte byteReceived, byteSend;


MeMegaPiDCMotor motorLeft(PORT2B);
MeMegaPiDCMotor motorRight(PORT3B);
MeUltrasonicSensor ultrasonic(PORT_8);
MeGyro gyro(PORT_7);

// Move function
bool moveForward() {
  int intialDistance = ultrasonic.distanceCm();
  bool done = false;
  bool abort = false;
  bool terrain = false;

  motorLeft.run(SPEED);
  motorRight.run(-SPEED);

  while(!done && !abort){
    delay(10);
    done = intialDistance - MOVE_DISTANCE < ultrasonic.distanceCm();
    abort = color.ReturnGrayscale() < 500;
    terrain = color.ReturnBlueData() > 500 && !abort;
  }

  while(abort){
    motorLeft.run(-SPEED);
    motorRight.run(SPEED);
    abort = ultrasonic.distanceCm() >= intialDistance;
  }
  return false;

  motorLeft.stop();
  motorRight.stop();

  if(terrain){
    delay(TERRAIN_WAIT);
  }

  return true;
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

// function for setting up the board as an SPI peripheral, so it can talk to the ESPs
int spi_init(){
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);

  SPCR |= _BV(SPE); // control register stuff i dont understand
                    // turns on spi in slave mode or something
  received = false;

  SPI.attachInterrupt();
}

// thing that runs when it receives an spi call?
ISR(SPI_STC_vect){
  byteReceived = SPDR; // store the incomming value
  received = true;     // yep
}

void setup(){

}

void loop(){

  moveForward();

  if (received)
  {
    switch (byteReceived)
    {
    case 0: // received 0, move 1 space
      byteSend = moveForward();
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

    default:
      break;
    }

    received = false;
  }

SPDR = byteSend; // sends data back to the master device
delay(10);
}
