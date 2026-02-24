#include <SPI.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>


/************ Radio Setup ***************/

// Change to 434.0 or other frequency, must match RX's freq!
#define RF69_FREQ 868.0

// Who am i? (server address)
#define MY_ADDRESS   36

// Where to send packets to! MY_ADDRESS in client (RX) should match this.
#define DEST_ADDRESS 24

#define RFM69_CS   16
#define RFM69_INT  21
#define RFM69_RST  17
#define LED        LED_BUILTIN

// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram rf69_manager(rf69, MY_ADDRESS);

int16_t packetnum = 0;  // packet counter, we increment per xmission

/*********** GPS Setup */
// Define pins for SoftwareSerial
const int RX_Pin = 1;
const int TX_Pin = 0;

const int GPS_Baud_Rate = 9600;

// Instantiate TinyGPS++ object
TinyGPSPlus gpsModule;

// Create software serial port named "gpsSerialPort"
SoftwareSerial gpsSerialPort(RX_Pin, TX_Pin);


void setup()
{
  // Initialize the hardware serial communication at 9600 baud
  Serial.begin(9600);

  // Initialize the software serial port at the GPS module's default baud rate
  gpsSerialPort.begin(GPS_Baud_Rate);
  
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

// Dont put this on the stack:
uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
uint8_t data[] = "  OK";

void loop(){
  radioLoop();
}

void radioLoop(){
  delay(1000);  // Wait 1 second between transmits, could also 'sleep' here!
  char radiopacket[50];
  snprintf(radiopacket, sizeof(radiopacket),"%.5f|%.5f", gpsModule.location.lat(),gpsModule.location.lng());
  itoa(packetnum++, radiopacket+13, 10);
  Serial.print("Sending ");
  Serial.println(radiopacket);
  // Send a message to the DESTINATION!
  if (rf69_manager.sendtoWait((uint8_t *)radiopacket, strlen(radiopacket), DEST_ADDRESS)) {
    // Now wait for a reply from the server
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (rf69_manager.recvfromAckTimeout(buf, &len, 2000, &from)) {
      buf[len] = 0; // zero out remaining string
      Blink(LED, 40, 3); // blink LED 3 times, 40ms between blinks
    } else {
      Serial.println("No reply, is anyone listening?");
    }
  } else {
    Serial.println("Sending failed (no ack)");
  }
}

void Blink(byte pin, byte delay_ms, byte loops) {
  while (loops--) {
    digitalWrite(pin, HIGH);
    delay(delay_ms);
    digitalWrite(pin, LOW);
    delay(delay_ms);
  }
}

void displayLocation(){
  if (gpsModule.location.isValid())
  {
    Serial.print("Latitude: ");
    Serial.println(gpsModule.location.lat(), 6); //print latitude
    Serial.print("Longitude: ");
    Serial.println(gpsModule.location.lng(), 6); //print longitude
    Serial.print("Altitude: ");
    Serial.println(gpsModule.altitude.meters()); //print altitude
  }
  else
  {
    Serial.println("Location: Unavailable");
  }
}

void displayGPSData()
{
  if (gpsModule.location.isValid())
  {
    Serial.print("Latitude: ");
    Serial.println(gpsModule.location.lat(), 6); //print latitude
    Serial.print("Longitude: ");
    Serial.println(gpsModule.location.lng(), 6); //print longitude
    Serial.print("Altitude: ");
    Serial.println(gpsModule.altitude.meters()); //print altitude
  }
  else
  {
    Serial.println("Location: Unavailable");
  }
  
  Serial.print("Current Date: ");
  if (gpsModule.date.isValid())
  {    
    Serial.print(gpsModule.date.day());    
    Serial.print("/");
    Serial.print(gpsModule.date.month());
    Serial.print("/");
    Serial.println(gpsModule.date.year());
  }
  else
  {
    Serial.println("Date Unavailable");
  }

  Serial.print("Current Time: ");
  if (gpsModule.time.isValid())
  {
    if (gpsModule.time.hour() < 10) Serial.print("0");
    Serial.print(gpsModule.time.hour());
    Serial.print(":");
    if (gpsModule.time.minute() < 10) Serial.print("0");
    Serial.print(gpsModule.time.minute());
    Serial.print(":");
    if (gpsModule.time.second() < 10) Serial.print("0");
    Serial.print(gpsModule.time.second());
    Serial.print(".");
    if (gpsModule.time.centisecond() < 10) Serial.print("0");
    Serial.println(gpsModule.time.centisecond());
  }
  else
  {
    Serial.println("Unavailable");
  }

  Serial.println();
  delay(1000);  // Delay before the next data display
}
