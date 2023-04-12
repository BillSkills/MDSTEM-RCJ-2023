#include <Arduino.h>
#include "esp_camera.h"

#define LED_BUILTIN 33
camera_fb_t * fb = NULL;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  esp_err_t res = ESP_OK;
  fb = esp_camera_fb_get();


  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(5000);
}