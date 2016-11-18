# Altitude_sensors_compare
Arduino utility to run side by side comparison of 2 atmospheric pressure sensors MS5611 and BME/BMP280 
Using respective drivers for these sensors available from here:
https://github.com/ebirdman/MS5611
and
https://github.com/ebirdman/SparkFun_BME280

Altitude_sensors_comp.ino
Version 1.0
Nov. 2016 Vladimir Blanshey

Arduino utility to run a side by side comparison 
two atmospheric pressure sensors: MS5611 and BME280/BMP280 
using similar oversampling setup for a high resolution pressure measurement. 
Primary goal is to analize the consistency of readings for 
aircraft avionics applications such as vario and altimeter.

Both respective libraries used for MS5611 and for BME/BMP280 can be downloaded from ebirdman github here.
The goal is to analize comparative precision, consistency and eventual drift over time 
of the reading in the same environmental condition (temperature and humidity) and within program's loop time.
Multiple experimental comparisons can be done using this sketch. Here is a sample list: 

- parallel drift of the readings over changed loop time (currently default is 5 sec loop)
- parallel drift of the readings over changed ambient temperature
- parallel drift of the readings over changed altitude
- parallel drift of the readings over time when environmental parameters stay the same

Sketch runs automatic comparison for the best chip in consistency of the multiple readings over the time period (which shows  surprisingly close results. Even for uncompensated mode of MS5611) .

MS5611 chip is configured for a maximum oversampling and readings of pressure are done with and without secondary compensation.
BME/BMP280 configured for a maximum oversampling of pressure reading (16x) and temperature (2x) as per Bosch datasheet.
Both sensors run over I2C interface.

Screenshot of output from the Serial monitor:

https://github.com/ebirdman/Altitude_sensors_compare/blob/master/Altitude_sensor_compare_output.JPG

Note:
Wire.begin() initialization must be included in the user's setup() routine prior to initialization of the sensor's objects.
I2C default address in respective libraries: 0x77 for MS5611 and 0x76 for BME280
