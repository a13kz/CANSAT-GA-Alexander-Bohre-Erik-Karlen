//Kod för att skriva på textfiler, skapa och förstöra textfiler.
//Koppiling på Rasperry pi feather rp2040: 

//3.3V och GND 
//CLK på SDkort komponent går till SCK på rp2040
//SO på komponent går till MISO på rp2040
//SI på komponent går till MOSI på rp2040
//CS på komponent gåår till RX på rp2040. Kan ändra men måste ändra chipSelect variabel också.

#include <Wire.h>

#include <SD.h>
#include <Adafruit_BMP085.h>
#define seaLevelPressure_hPa 1013.25

Adafruit_BMP085 bmp;
const int chipSelect = 9;
File myFile;

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

  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }
  //Om example.txt existerar. Ta bort den och skapa den igen med texten Hej 123

  //if (SD.exists("example.txt")) {
  //  Serial.println("example.txt exists. Removing it");
  //  SD.remove("example.txt");
  //} 

  //Serial.println("Creating example.txt...");
  //myFile = SD.open("example.txt", FILE_WRITE);
  //myFile.println("Hej 123");
  //myFile.println("Hej 124");
  //myFile.close();
  //myFile = SD.open("example.txt", FILE_WRITE);
  //myFile.println("Hej 125");
  //myFile.println("Hej 126");
  //myFile.close();

  digitalWrite(13,LOW);
  delay(500);
  digitalWrite(13,HIGH);
  delay(500);
  digitalWrite(13,LOW);
}

void loop() {
  //Serial.println("writing");
  myFile = SD.open("temp.txt", FILE_WRITE);
  //Serial.println(bmp.readTemperature());
  //myFile.println(bmp.readTemperature());

  myFile.print("Temperature = ");
  myFile.print(bmp.readTemperature());
  myFile.println(" *C");
  myFile.print("Pressure = ");
  myFile.print(bmp.readPressure());
  myFile.println(" Pa");
  myFile.print("Altitude = ");
  myFile.print(bmp.readAltitude());
  myFile.println(" meters");
  myFile.print("Pressure at sealevel (calculated) = ");
  myFile.print(bmp.readSealevelPressure());
  myFile.println(" Pa");
  myFile.print("Real altitude = ");
  myFile.print(bmp.readAltitude(seaLevelPressure_hPa * 100));
  myFile.println(" meters");
  myFile.println();
  myFile.close();
  delay(100);
}