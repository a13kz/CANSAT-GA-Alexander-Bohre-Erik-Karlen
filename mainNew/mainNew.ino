#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <SD.h>
#include <Adafruit_BMP085.h>
#define seaLevelPressure_hPa 1013.25
const int RX_Pin = 1;
const int TX_Pin = 0;
const int chipSelect = 9;
const int GPS_Baud_Rate = 9600;

TinyGPSPlus module;
Adafruit_BMP085 bmp;
Adafruit_MPU6050 mpu;

File file;

void setup() {

  // hardware serial com at 9600 baud
  Serial.begin(9600);
  delay(100);

  gpsSerialPort.begin(GPS_Baud_Rate);

  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);

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
  if (!mpu.begin()) {
  Serial.println("Failed to find MPU6050 chip");
  while (1) {
    delay(10);
  }
  }
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
  Serial.println();
  delay(100);
  //Om example.txt existerar. Ta bort den och skapa den igen med texten Hej 123
char i = 0;
while (SD.exists("reading"+i+".txt")) {
  i+=1;
}
  digitalWrite(13,LOW);
  delay(500);
  digitalWrite(13,HIGH);
  delay(500);
  digitalWrite(13,LOW);
}

void loop() {
  readWrite();

}

void displayLocation(){
  if (module.location.isValid())
  {
    Serial.print("Latitude: ");
    Serial.println(module.location.lat(), 6); //print latitude
    Serial.print("Longitude: ");
    Serial.println(module.location.lng(), 6); //print longitude
    Serial.print("Altitude: ");
    Serial.println(module.altitude.meters()); //print altitude
  }
  else
  {
    Serial.println("Location: Unavailable");
  }
  delay(100);
}

void readWrite(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Serial.println("Reading...");
  temp = SD.open("temp.txt", FILE_WRITE);
  pressure = SD.open("pressure.txt", FILE_WRITE);
  altitude = SD.open("altitude.txt", FILE_WRITE);
  movement = SD.open("movement.txt", FILE_WRITE);
  
  temp.print("Temperature = ");
  temp.print(bmp.readTemperature());
  temp.println(" *C");
  
  pressure.print("Pressure = ");
  pressure.print(bmp.readPressure());
  pressure.println(" Pa");
  pressure.println();
  pressure.print("Pressure at sealevel (calculated) = ");
  pressure.print(bmp.readSealevelPressure());
  pressure.println(" Pa");
  
  altitude.print("Altitude = ");
  altitude.print(bmp.readAltitude());
  altitude.println(" meters");
  altitude.println();
  
  altitude.print("Real altitude = ");
  altitude.print(bmp.readAltitude(seaLevelPressure_hPa * 100));
  altitude.println(" meters");

  movement.print("Acceleration X: ");
  movement.print(a.acceleration.x);
  movement.print(", Y: ");
  movement.print(a.acceleration.y);
  movement.print(", Z: ");
  movement.print(a.acceleration.z);
  movement.println(" m/s^2");
  movement.println();
  movement.print("Rotation X: ");
  movement.print(g.gyro.x);
  movement.print(", Y: ");
  movement.print(g.gyro.y);
  movement.print(", Z: ");
  movement.print(g.gyro.z);
  movement.println(" rad/s");
  
  temp.close();
  pressure.close();
  altitude.close();
  movement.close();
  delay(100);
}