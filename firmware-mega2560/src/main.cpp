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

float gyroCorrection = 0;
float currentGyroCorrection = 0;

int direction = 1; // west, north, east, south


MeMegaPiDCMotor motorLeft(PORT2B);
MeMegaPiDCMotor motorRight(PORT3B);
MeUltrasonicSensor ultrasonic(PORT_6);
MeGyro gyro(PORT_7);

// Move function
bool moveForward() {
  int intialDistance = ultrasonic.distanceCm();
  bool done = false;
  bool abort = false;
  bool terrain = false;

  motorLeft.run(SPEED);
  motorRight.run(-SPEED);

  while(ultrasonic.distanceCm() > intialDistance - MOVE_DISTANCE){
    delay(10);
  }

  // while(abort){
  //   motorLeft.run(-SPEED);
  //   motorRight.run(SPEED);
  //   abort = ultrasonic.distanceCm() >= intialDistance;
  // }
  // return false;

  motorLeft.stop();
  motorRight.stop();

  // if(terrain){
  //   delay(TERRAIN_WAIT);
  // }

  return true;
}

// Right turn
void turnRight() {
  gyro.begin();

  motorLeft.run(150);
  motorRight.run(150);

  while(gyro.getAngleZ() > -88){
    gyro.update();
  }

  motorLeft.stop();
  motorRight.stop();


  // keep track of the absolutedirection
  switch (direction)
  {
  case 3:
    direction = 0;
    break;
  default:
    direction ++;
    break;
  }
}


// Left turn
void turnLeft() {

  gyro.begin();

  motorLeft.run(-150);
  motorRight.run(-150);

  while(gyro.getAngleZ() < 88){
    gyro.update();
  }

  motorLeft.stop();
  motorRight.stop();


  // keep track of the absolutedirection
  switch (direction)
  {
  case 0:
    direction = 3;
    break;
  default:
    direction --;
    break;
  }
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
  Serial.begin(9600);
}

void loop(){

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
    case 3: // received 3, return whether there is a wall in front
      byteSend = ultrasonic.distanceCm() > 40;
      break;

    default:
      break;
    }

    received = false;
  }

SPDR = byteSend; // sends data back to the master device
delay(10);
}
