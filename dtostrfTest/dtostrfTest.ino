#include <Wire.h>
#include <Adafruit_BMP085.h>
#define seaLevelPressure_hPa 1013.25

Adafruit_BMP085 bmp;
  
void setup() {
  Serial.begin(9600);
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }
}
  
void loop() {
  char tempArr[20];
  dtostrf(bmp.readTemperature(),5,3, tempArr);
  Serial.println(tempArr);
  delay(500);
}