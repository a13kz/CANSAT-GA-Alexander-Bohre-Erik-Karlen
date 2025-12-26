#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <SPI.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>

#define seaLevelPressure_hPa 1013.25

// frequency setup 868.0 is the standard in Europe
#define RF69_FREQ 868.0

// Who am i? (server address)
#define MY_ADDRESS   36

// Where to send packets to! MY_ADDRESS in client (RX) should match this.
#define DEST_ADDRESS 24

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

RHReliableDatagram rf69_manager(rf69, MY_ADDRESS);

int16_t packetnum = 0; 

void setup(void) {
  Serial.begin(115200);
  radioSetup();
  while(!Serial){
    delay(10);
  }
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

void radioSetup(){
  //while (!Serial) delay(1); // Wait for Serial Console (comment out line if no computer)

  pinMode(LED, OUTPUT);
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  Serial.println("Feather Addressed RFM69 TX Test!");
  Serial.println();

  // manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);

  if (!rf69_manager.init()) {
    Serial.println("RFM69 radio init failed");
    while (1);
  }
  Serial.println("RFM69 radio init OK!");
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  }

  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

  // The encryption key has to be the same as the one in the client
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
  rf69.setEncryptionKey(key);

  Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");

}
uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
uint8_t data[] = "  OK";

void radio(){
 delay(1000);  // Wait 1 second between transmits, could also 'sleep' here!

  char radiopacket[20] = "Hello World #";
  itoa(packetnum++, radiopacket+13, 10);
  Serial.print("Sending "); Serial.println(radiopacket);

  // Send a message to the DESTINATION!
  if (rf69_manager.sendtoWait((uint8_t *)radiopacket, strlen(radiopacket), DEST_ADDRESS)) {
    // Now wait for a reply from the server
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (rf69_manager.recvfromAckTimeout(buf, &len, 2000, &from)) {
      buf[len] = 0; // zero out remaining string

      Serial.print("Got reply from #"); Serial.print(from);
      Serial.print(" [RSSI :");
      Serial.print(rf69.lastRssi());
      Serial.print("] : ");
      Serial.println((char*)buf);
      Blink(LED, 40, 3); // blink LED 3 times, 40ms between blinks
    } else {
      Serial.println("No reply, is anyone listening?");
    }
  } else {
    Serial.println("Sending failed (no ack)");
  }
}



void loop() {
  // put your main code here, to run repeatedly:
  //MPU180Handling();
  //BMPHandling();
  radio();
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
  Serial.print("|");
  Serial.print(bmp.readPressure());
  Serial.print("|");
  Serial.print(bmp.readAltitude(seaLevelPressure_hPa * 100));
  Serial.print("|");
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.print(" m/s^2");
  Serial.print("|");
  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

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

//void test(){
//  sensors_event_t a, g, temp;
//  mpu.getEvent(&a, &g, &temp);
//  //output = (String(bmp.readTemperature(),2))+" | "+String(bmp.readPressure(),7)+" | "+(String(bmp.readAltitude(seaLevelPressure_hPa * 100),2))+" | Acceleration X: "+String(a.acceleration.x,2)+", Y: "+String(a.acceleration.y,2)+", Z: "+String(a.acceleration.z,2)+"m/s^2 | Rotation X: " +String(g.gyro.x,2)+ ", Y: " +String(g.gyro.y,2)+ ", Z: " +String(g.gyro.z,2)+" rad/s";
//  output1 = (String(bmp.readTemperature(),2))+" | "+String(bmp.readPressure(),7);
//  char radiopacket[20];
//  (String(bmp.readTemperature(),2))+" | "+String(bmp.readPressure(),7).toCharArray(radiopacket,20);
//
//  Serial.println(radiopacket);
//}




  //output[] = (String(bmp.readTemperature(),2))+" | "+String(bmp.readPressure(),7)+" | "+(String(bmp.readAltitude(seaLevelPressure_hPa * 100),2))+" | Acceleration X: "+String(a.acceleration.x,2)+", Y: "+String(a.acceleration.y,2)+", Z: "+String(a.acceleration.z,2)+"m/s^2 | Rotation X: " +String(g.gyro.x,2)+ ", Y: " +String(g.gyro.y,2)+ ", Z: " +String(g.gyro.z,2)+" rad/s";
  //char radiopacket[100];
  


void Blink(byte pin, byte delay_ms, byte loops) {
  while (loops--) {
    digitalWrite(pin, HIGH);
    delay(delay_ms);
    digitalWrite(pin, LOW);
    delay(delay_ms);
  }
}