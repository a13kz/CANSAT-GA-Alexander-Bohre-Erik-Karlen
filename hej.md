## Background

Background: The following project uses two adafruit rp2060 rfm69 microcontroller to collect datapoints and transfers it from the atmospheric probe to the ground station. The project is using the BMP180 to collect temperature and pressure data, the MPU6050 to collect accelerations and movements in general and the gy-gps6mv2 to collect geolocation data. The project uses radiocommunication via the built in rfm69 module with a range of up to 200 meters and saves collected data with the adafruit microSD breakout board.

## Usage

For usage: The transmitter module is to have the TX_main code while the receiver should have the RX_radiomodule.

On start, the atmospheric probe starts right away and writes the collected data onto the connected SD-card. The radio from start continuesly provide latitude and longitude of the probe which are presented in the ground station's terminal.

---

## Code Structure

The project uses two programs:  **receiver (ground station)** and  **transmitter (atmospheric probe)**.

### Receiver (`RX_radiomodule`)

The receiver is listens for incoming radio transmission and displays collected geolocation data in terminal.

**Key features:**
- Uses `RH_RF69` and `RHReliableDatagram` for reliable communication  
- Configured with address `24`  
- Continuously listens for incoming packets  
- Outputs received GPS data to the serial terminal  
- Uses onboard LED as a visual indicator when data is received  

**Behavior:**
- Waits for incoming packets  
- When data is received:
  - Identifies the sender  
  - Prints the message to the serial monitor

---

### Transmitter (`TX_main`)

The transmitter is put on the atmospheric probe, collecting sensor data, storing it locally, and transmitts geolocation data.

**Key features:**
- Configured with address `36`, sending to receiver address `24`  
- Integrates multiple sensors:
  - BMP180 (temperature, pressure, altitude)  
  - MPU6050 (acceleration and gyroscope data)  
  - GY-GPS6MV2 (GPS positioning)  
- Logs sensor data to SD card (`new_reading.txt`)  
- Sends GPS coordinates via RFM69 radio  
- Uses AES encryption for communication  

**Behavior:**
- On startup:
  - Initializes sensors, SD card, GPS, and radio module  
- Main loop:
  - `radioLoop()`:
    - Sends GPS coordinates every second  
    - Waits for acknowledgment from receiver  
  - `SDWrite()`:
    - Reads sensor values  
    - Writes data to SD card  

---

## Communication Protocol

- Frequency: `868 MHz` (must match on both devices)  
- Encryption: Enabled (shared 16-byte key)  
- Transmission: Reliable datagrams with acknowledgment  

## Wiring

### I2C Sensors (BMP180 & MPU6050)
Both sensors use the I2C bus and can share the same connections:

- **VCC** → 3.3V  
- **GND** → GND  
- **SDA** → SDA (microcontroller I2C data pin)  
- **SCL** → SCL (microcontroller I2C clock pin)  

---

### GPS Module (GY-GPS6MV2)
Connected via software serial:

- **VCC** → 3.3V or 5V (depending on module)  
- **GND** → GND  
- **TX (GPS)** → Pin 6 (RX on microcontroller)  
- **RX (GPS)** → Pin 5 (TX on microcontroller)  

---

### RFM69 Radio Module
(Already integrated on Adafruit board, but pin references:)

- **CS** → Pin 16  
- **INT** → Pin 21  
- **RST** → Pin 17  
- **SPI pins** → Hardware SPI (MOSI, MISO, SCK)  

---

### microSD Breakout Board

- **CS** → Pin 9  
- **MOSI** → SPI MOSI  
- **MISO** → SPI MISO  
- **SCK** → SPI SCK  
- **VCC** → 3.3V  
- **GND** → GND  

---

## Notes

- Ensure both radio modules use the same frequency (`868 MHz`) and encryption key  
- Double-check power requirements (some modules require stable 3.3V)  
- MicroSD card have to be inserted for module to function
