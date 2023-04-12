/*
main.cpp (MegaPi)
April 12, 2023
MDSTEM-RCJ-2023
Usan Siriwardana, William Zheng
*/

// Includes
#include <Adafruit_TCS34725.h>
#include <Arduino.h>
#include <MeMegaPi.h>
#include <SPI.h>
#include <Wire.h>

// Defines
#define MOVE_SPEED 200
#define MOVE_DISTANCE 30
#define TURN_SPEED 100
#define WALL_DISTANCE 9
#define TERRAIN_WAIT 5000
#define MS_DELAY 10

// Sensor ports
MeMegaPiDCMotor motorLeft(PORT2B);
MeMegaPiDCMotor motorRight(PORT3B);
MeUltrasonicSensor ultrasonic(PORT_6);
MeGyro gyro(PORT_7);
// MeColorSensor color(PORT_8);
// Adafruit_TCS34725 color = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

/* VARIABLES */
volatile bool received;
volatile byte byteReceived, byteSend;
float gyroCorrection = 0;
float currentGyroCorrection = 0;
int direction = 1; // west, north, east, south
float r, g, b;

class helpers {

  public:
    static double round(double num, int mult = 90) {
      num += 180;
      int result = num + mult/2;
      result -= result % mult;
      result -= 180;
      return result;
    }
    float y(float x)
    {
      return 1/(1+x*x);
    }
 
    float trapezoidal(float a, float b, float n)
    {
      float h = (b-a)/n;
  
      float s = y(a)+y(b);
  
      for (int i = 1; i < n; i++)
          s += 2*y(a+i*h);
  
      return (h/2)*s;
    }

};


bool wallDetect() { return ultrasonic.distanceCm() > WALL_DISTANCE; }

// Move function
bool move() {
  int intialDistance = ultrasonic.distanceCm();
  bool done = false;
  bool abort = false;
  bool terrain = false;

  motorLeft.run(MOVE_SPEED);
  motorRight.run(-MOVE_SPEED);

  while (!done) {
    done = !wallDetect();
    // color.getRGB(&r, &g, &b);

    // abort = r < 50 && g < 50 && b < 50;
    // terrain = b > 50 && !abort;

    delay(10);
  }

  // if (abort) {
  //   while (abort) {
  //     motorLeft.run(-MOVE_SPEED);
  //     motorRight.run(MOVE_SPEED);
  //     abort = ultrasonic.distanceCm() >= intialDistance;
  //   }
  //   return false;
  // }

  motorLeft.stop();
  motorRight.stop();

  delay(100);

  // if (terrain) {
  //   delay(TERRAIN_WAIT);
  // }

  return true;
}


// Right turn
void turnRight() {
  motorLeft.run(TURN_SPEED);
  motorRight.run(TURN_SPEED);

  int target = helpers::round(gyro.getAngleZ()) - 90;
  if (target < -180) target += 360;

  Serial.println(target);

  while (gyro.getAngleZ() > target) {
    gyro.update();
  }

  motorLeft.stop();
  motorRight.stop();

  gyro.begin();

  // keep track of the absolutedirection
  // switch (direction) {
  // case 3:
  //   direction = 0;
  //   break;
  // default:
  //   direction++;
  //   break;
  // }
}

// Left turn
void turnLeft() {
  motorLeft.run(-TURN_SPEED);
  motorRight.run(-TURN_SPEED);

  int target = helpers::round(gyro.getAngleZ()) + 90;
  if (target > 180) target -= 360;

  Serial.println(target);

  while (gyro.getAngleZ() < target) {
    gyro.update();
  }

  motorLeft.stop();
  motorRight.stop();

  gyro.begin();

  // keep track of the absolutedirection
  // switch (direction) {
  // case 0:
  //   direction = 3;
  //   break;
  // default:
  //   direction--;
  //   break;
  // }
}

// Function for setting up the board as an SPI peripheral, so it can talk to the ESPs
int spi_init() {
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);

  SPCR |= _BV(SPE); // Control register stuff I don't understand
                    // Turns on SPI in slave mode or something
  received = false;

  SPI.attachInterrupt();
}

// Thing that runs when it receives an SPI call?
ISR(SPI_STC_vect) {
  byteReceived = SPDR; // Store the incoming value
  received = true;
}

void setup() {
  spi_init();
  Serial.begin(9600);
  gyro.begin();
}

int randInt = random(4);
bool open[3];
bool turnedLeft = false;

void loop() {

  // turnLeft();
  // gyro.update();
  // Serial.println(gyro.getAngleZ());
  // delay(1000);
  // turnRight();
  // gyro.update();
  // Serial.println(gyro.getAngleZ());
  // delay(1000);
  // turnLeft();
  // gyro.update();
  // Serial.println(gyro.getAngleZ());
  // delay(1000);

  /*color.getRGB(&r, &g, &b);

  Serial.print(r);
  Serial.print(" ");
  Serial.print(g);
  Serial.print(" ");
  Serial.println(b);*/

  // if (received) {
  //   switch (byteReceived) {
  //   case 0:
  //     byteSend = moveForward();
  //     break;
  //   case 1:
  //     turnLeft();
  //     break;
  //   case 2:
  //     turnRight();
  //     break;
  //   case 3:
  //     byteSend = ultrasonic.distanceCm() > 40;
  //     break;

  //   default:
  //     break;
  //   }
  //   received = false;
  // }
  // SPDR = byteSend;
  // delay(MS_DELAY);

  // turnLeft();
  // for (int i = 0; i < 3; i++)
  // {
  //   open[i] = wallDetect();
  //   turnRight();
  // }

  // while(!open[randInt]){
  //   randInt = random(4);
  // }

  // switch (randInt)
  // {
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

  // if (wallDetect()) {
  //   move();
  //   turnedLeft = !turnedLeft;
  // } else {
  //   if (turnedLeft) {
  //     while (!wallDetect()) {
  //       turnRight();
  //     }
  //   } else {
  //     while (!wallDetect()) {
  //       turnLeft();
  //     }
  //   }
  // }

  if (wallDetect()) {
    move();
  } else {
    int num = rand();
    if(num%2 == 0){
      turnLeft();
    } else {
      turnRight();
    }
  }

  
}