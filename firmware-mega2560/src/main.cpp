#include <Arduino.h>
#include <Wire.h>
#include <MeMegaPi.h>

MeMegaPiDCMotor motorL(PORT2B);
MeMegaPiDCMotor motorR(PORT1B);

void setup()
{
}

void loop()
{
  // put your main code here, to run repeatedly:

  motorL.run(-150);
  motorR.run(150);
  delay(4000);
}