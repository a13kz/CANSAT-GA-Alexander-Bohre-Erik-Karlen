# gymnasie_arbete
Background:
The following project uses two adafruit rp2060 rfm69 microcontroller to collect datapoints and transfer *it from the atmospheric probe to the ground station. The project is using the BMP180 to collect temperature and pressure data, the MPU6050 to collect accelerations and movements in general and the gy-gps6mv2 to collect geolocation data. The project uses radiocommunication via the built in rfm69 module with a range of up to 200 meters and saves collected data with the adafruit microSD breakout board.

For usage:
The transmitter module is to have the TX_main code while the receiver should have the RX_radiomodule. 

On start the atmospheric probe starts right away and writes the collected data onto the connected SD-card. The radio from start continuesly provide latitude and longitude of the probe which are presented in the ground station's terminal. 
