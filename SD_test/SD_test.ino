//Kod för att skriva på textfiler, skapa och förstöra textfiler.
//Koppiling på Rasperry pi feather rp2040: 

//3.3V och GND 
//CLK på SDkort komponent går till SCK på rp2040
//SO på komponent går till MISO på rp2040
//SI på komponent går till MOSI på rp2040
//CS på komponent gåår till RX på rp2040. Kan ändra men måste ändra chipSelect variabel också.

#include <Wire.h>
//#include <Adafruit_MPU6050.h>
//#include <Adafruit_Sensor.h>
#include <SD.h>
//#include <Adafruit_BMP085.h>
//#define seaLevelPressure_hPa 1013.25

//Adafruit_BMP085 bmp;
//Adafruit_MPU6050 mpu;
const int chipSelect = 9;
File file;

void setup() {
  Serial.begin(9600);
  delay(100);

  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  delay(1000);

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is your wiring correct?");
    Serial.println("3. did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset button on the board and reopen this serial monitor after fixing your issue!");
    while (1);
  }

  Serial.println("initialization done.");
}
int i = 0;
void loop() {
  file = SD.open("reading.txt", FILE_WRITE);
  Serial.println("Reading...");
  file.close();
  delay(100);
}