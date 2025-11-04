#include <Wire.h>
#include <Adafruit_BMP180.h>
#define seaLevelPressure_hPa 1013.25

Adafruit_BMP180 bmp;
float temp = 0;
float pressure = 0;
float alt = 0;

void setup() {
  
  Serial.begin(9600);
  if (!bmp.begin()){
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  }
}

void loop() {
  pressure = bmp.readPressure();
  temp = bmp.readTemperature();
  alt = bmp.readAltitude();
}
