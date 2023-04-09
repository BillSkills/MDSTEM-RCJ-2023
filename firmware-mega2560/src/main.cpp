#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <MeMegaPi.h>

#define SPEED 200
#define MOVE_DISTANCE 30

volatile bool received;
volatile byte byteReceived, byteSend;


MeMegaPiDCMotor motorLeft(PORT1B);
MeMegaPiDCMotor motorRight(PORT2B);
MeUltrasonicSensor ultrasonic(PORT_8);
MeUltrasonicSensor ultrasonicLeft(PORT_7);
MeUltrasonicSensor ultrasonicRight(PORT_6);
MeGyro gyro(PORT_5);

void move()
{
  int intialDistance = ultrasonic.distanceCm();
  bool done = false;
  motorLeft.run(SPEED);
  motorRight.run(-SPEED);
  while (!done)
  {
    delay(10);
    done = intialDistance - MOVE_DISTANCE < ultrasonic.distanceCm();
  }

  // delay(distance*100);

  motorLeft.stop();
  motorRight.stop();
}

void turnRight()
{
  int initialZ = gyro.getAngleZ();
  motorLeft.run(50);
  motorRight.run(-50);
  while (initialZ + 90 > gyro.getAngleZ())
  {
    delay(10);
  }
  motorLeft.stop();
  motorRight.stop();
}

void turnLeft()
{
  int initialZ = gyro.getAngleZ();
  motorLeft.run(-50);
  motorRight.run(50);
  while (initialZ - 90 > gyro.getAngleZ())
  {
    delay(10);
  }
  motorLeft.stop();
  motorRight.stop();
}

// function for setting up the board as an SPI peripheral, so it can talk to the ESPs
int spi_init()
{
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);

  SPCR |= _BV(SPE); // control register stuff i dont understand
                    // turns on spi in slave mode or something
  received = false;

  SPI.attachInterrupt();
}

// thing that runs when it receives an spi call?
ISR(SPI_STC_vect)
{
  byteReceived = SPDR; // store the incomming value
  received = true;     // yep
}

void setup()
{

}

void loop()
{

  if (received)
  {
    switch (byteReceived)
    {
    case 0: // received 0, move 1 space
      move();
      break;
    case 1: // received 1, turn left
      turnLeft();
      break;
    case 2: // received 2, turn right
      turnRight();
      break;
    case 3: // received 3, return measurment from front ultrasonic
      byteSend = ultrasonic.distanceCm();
      break;
    case 4: // received 4, return measurment from front ultrasonic
      byteSend = ultrasonicLeft.distanceCm();
      break;
    case 5: // received 5, return measurment from front ultrasonic
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