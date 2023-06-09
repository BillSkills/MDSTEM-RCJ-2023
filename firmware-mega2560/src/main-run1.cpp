// /*
// main.cpp (MegaPi)
// April 12, 2023
// MDSTEM-RCJ-2023
// Usan Siriwardana, William Zheng
// */

// // Includes
// #include <Adafruit_TCS34725.h>
// #include <Arduino.h>
// #include <MeMegaPi.h>
// #include <SPI.h>
// #include <Wire.h>

// // Defines
// #define MOVE_SPEED 200
// #define MOVE_DISTANCE 30
// #define TURN_SPEED 150
// #define WALL_DISTANCE 15
// #define TERRAIN_WAIT 5000
// #define MS_DELAY 10

// // Sensor ports
// MeMegaPiDCMotor motorLeft(PORT2B);
// MeMegaPiDCMotor motorRight(PORT3B);
// MeUltrasonicSensor ultrasonic(PORT_6);
// MeGyro gyro(PORT_7);
// // Adafruit_TCS34725 color = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

// /* VARIABLES */
// volatile bool received;
// volatile byte byteReceived, byteSend;
// float gyroCorrection = 0;
// float currentGyroCorrection = 0;
// int direction = 1; // west, north, east, south
// float r, g, b;

// // Move function
// bool move() {
//   int intialDistance = ultrasonic.distanceCm();
//   bool done = false;
//   bool abort = false;
//   bool terrain = false;

//   motorLeft.run(MOVE_SPEED);
//   motorRight.run(-MOVE_SPEED);

//   while (!done) {
//     done = ultrasonic.distanceCm() > intialDistance - MOVE_DISTANCE;
//     // color.getRGB(&r, &g, &b);

//     // abort = r < 50 && g < 50 && b < 50;
//     // terrain = b > 50 && !abort;

//     delay(10);
//   }

//   // if (abort) {
//   //   while (abort) {
//   //     motorLeft.run(-MOVE_SPEED);
//   //     motorRight.run(MOVE_SPEED);
//   //     abort = ultrasonic.distanceCm() >= intialDistance;
//   //   }
//   //   return false;
//   // }

//   motorLeft.stop();
//   motorRight.stop();

//   // if (terrain) {
//   //   delay(TERRAIN_WAIT);
//   // }

//   return true;
// }


// bool wallDetect() { return ultrasonic.distanceCm() > WALL_DISTANCE; }

// // Right turn
// void turnRight() {
//   gyro.begin();

//   motorLeft.run(TURN_SPEED);
//   motorRight.run(TURN_SPEED);

//   while (gyro.getAngleZ() > -90) {
//     gyro.update();
//   }

//   motorLeft.stop();
//   motorRight.stop();

//   // keep track of the absolutedirection
//   // switch (direction) {
//   // case 3:
//   //   direction = 0;
//   //   break;
//   // default:
//   //   direction++;
//   //   break;
//   // }
// }

// // Left turn
// void turnLeft() {
//   gyro.begin();

//   motorLeft.run(-TURN_SPEED);
//   motorRight.run(-TURN_SPEED);

//   while (gyro.getAngleZ() < 90) {
//     gyro.update();
//   }

//   motorLeft.stop();
//   motorRight.stop();

//   // keep track of the absolutedirection
//   // switch (direction) {
//   // case 0:
//   //   direction = 3;
//   //   break;
//   // default:
//   //   direction--;
//   //   break;
//   // }
// }

// // Function for setting up the board as an SPI peripheral, so it can talk to the ESPs
// int spi_init() {
//   pinMode(MISO, OUTPUT);
//   pinMode(MOSI, INPUT);

//   SPCR |= _BV(SPE); // Control register stuff I don't understand
//                     // Turns on SPI in slave mode or something
//   received = false;

//   SPI.attachInterrupt();
// }

// // Thing that runs when it receives an SPI call?
// ISR(SPI_STC_vect) {
//   byteReceived = SPDR; // Store the incoming value
//   received = true;
// }

// void setup() {
//   spi_init();
//   // color.begin();
//   Serial.begin(9600);
// }

// int randInt = random(4);
// bool open[3];
// bool turnedLeft = false;

// void loop() {
//   // color.getRGB(&r, &g, &b);

//   // Serial.print(r);
//   // Serial.print(" ");
//   // Serial.print(g);
//   // Serial.print(" ");
//   // Serial.println(b);

//   // if (received) {
//   //   switch (byteReceived) {
//   //   case 0:
//   //     byteSend = moveForward();
//   //     break;
//   //   case 1:
//   //     turnLeft();
//   //     break;
//   //   case 2:
//   //     turnRight();
//   //     break;
//   //   case 3:
//   //     byteSend = ultrasonic.distanceCm() > 40;
//   //     break;

//   //   default:
//   //     break;
//   //   }
//   //   received = false;
//   // }
//   // SPDR = byteSend;
//   // delay(MS_DELAY);

//   // turnLeft();
//   // for (int i = 0; i < 3; i++)
//   // {
//   //   open[i] = wallDetect();
//   //   turnRight();
//   // }
  
//   // while(!open[randInt]){
//   //   randInt = random(4);
//   // }

//   // switch (randInt)
//   // {
//   // case 0:
//   //   turnLeft();
//   //   turnLeft();
//   //   move();
//   //   break;
//   // case 1:
//   //   turnLeft();
//   //   move();
//   //   break;
//   // case 2:
//   //   move();
//   //   break;
  
//   // default:
//   //   break;
//   // }

//   if(wallDetect()){
//     move();
//   } else{
//     turnLeft();
//   }
// }
