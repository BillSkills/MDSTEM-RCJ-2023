#include <Arduino.h>
#include <Servo.h>

#define SERVO_PIN         GPIO_NUM_13

Servo servo1;
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);

  servo1.attach(SERVO_PIN);

}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(LED_BUILTIN, HIGH);
  servo1.write(180);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  servo1.write(0);
  delay(1000);
}