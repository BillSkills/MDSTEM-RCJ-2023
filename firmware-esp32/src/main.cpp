#include <Arduino.h>
#include <Servo.h>
#include <SPI.h>

#define SERVO_PIN         GPIO_NUM_13

Servo servo1;

int rescueKits = 12;

void deployKit(){ // deploy 1 rescue kit
  if(rescueKits>0){
    servo1.write(180);
    delay(100);
    servo1.write(0);
    rescueKits --;
  }
}

void deployKit(int numKits){  // deploy multiple rescue kits
  for (int i = 0; i < numKits; i++)
  {
    deployKit();
  }
  
}

void move(){
  SPI.transfer(0);
}

void turnLeft(){
  SPI.transfer(1);
}

void turnRight(){
  SPI.transfer(2);
}

int ultrasonic(){
  return SPI.transfer(3);
}

int ultrasonicLeft(){
  return SPI.transfer(4);
}

int ultrasonicRight(){
  return SPI.transfer(5);
}

void setup() {
  // put your setup code here, to run once:
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
  // put your main code here, to run repeatedly:

}