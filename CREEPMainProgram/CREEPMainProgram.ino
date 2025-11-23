#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <SPI.h>
#include <RH_RF69.h>

#define seaLevelPressure_hPa 1013.25

// frequency setup 868.0 is the standard in Europe
#define RF69_FREQ 868.0

// define radio pins
#define RFM69_CS   16
#define RFM69_INT  21
#define RFM69_RST  17
#define LED        LED_BUILTIN

//const int length = 100;
//float averageTemp = [length];
//float averageAlt = [length];

Adafruit_MPU6050 mpu;
Adafruit_BMP085 bmp;

//instance of radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

void setup(void) {
  Serial.begin(115200);
  while(!Serial)
    delay(10);
  //Serial.println("Serial start");

  //init
  if (!mpu.begin()){
    Serial.println("MPU6050 could not be found!");
    while (1) {
      delay(10);
    }
  }
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

  // Components found
  //Serial.println("MPU6050 found!");
  //Serial.println("BMP85 found!");

  // Accelerometer setup
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  //Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    //Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    //Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    //Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    //Serial.println("+-16G");
    break;
  }

  // Gyro setup
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  //Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    //Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    //Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    //Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    //Serial.println("+- 2000 deg/s");
    break;
  }

  // Bandwidth setup
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  //Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    //Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    //Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    //Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    //Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    //Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    //Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    //Serial.println("5 Hz");
    break;
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //MPU180Handling();
  BMPHandling();
}

void RFMHandling(){
  delay(1000);

  char radiopacket[20] = "Hello World #";
  // send message
  rf69.send((uint8_t *)radiopacket, strlen(radiopacket));
  rf69.waitPacketSent();
}

void MPU180Handling(){
  Serial.println("MPU");
    /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.println("");
  delay(500);
}

void BMPHandling(){
  //Serial.println("BMP");
  //Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.print(" ");
  Serial.print(bmp.readPressure());
  Serial.print(" ");
  Serial.println(bmp.readAltitude(seaLevelPressure_hPa * 100));
  //Serial.println(" *C");
  delay(500);
//  Serial.print("Pressure = ");
//  Serial.print(bmp.readPressure());
//  Serial.println(" Pa");
//
//
//  Serial.print("Altitude = ");
//  Serial.print(bmp.readAltitude());
//  Serial.println(" meters");
//
//
//  Serial.print("Pressure at sealevel (calculated) = ");
//  Serial.print(bmp.readSealevelPressure());
//  Serial.println(" Pa");
//
//
//  Serial.print("Real altitude = ");
//  Serial.print(bmp.readAltitude(seaLevelPressure_hPa * 100));
//  Serial.println(" meters");

}

float celciusToKelvin(float temp){
  return temp+273.15;
}

void average_temp(){
  //Serial.print(averageTemp/length)
  //Serial.println(averagrAlt/length)
}

