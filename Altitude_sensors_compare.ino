/*****************************************************
Altitude_sensors_comp.ino
Version 1.0
Nov. 2016 Vladimir Blanshey

Arduino utility to run for a comparison side by side 
two atmospheric pressure sensors: MS5611 and BME280/BMP280 
using similar oversampling setup for a high resolution pressure measurement. 
Primary goal is to analize the consistency of readings for 
aircraft avionics application such as vario and altimeter.

This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details  <http://www.gnu.org/licenses/>.
******************************************************/
#include <Wire.h>
#include <MS5611.h>
#include "SparkFunBME280.h"

MS5611 ms5611;
BME280 bme;

float slp = 102000.00; // sea level pressure

// to calculate signal consistency
float maxMS5611u, minMS5611u;
float maxMS5611c, minMS5611c;
float maxBME280, minBME280;

long Pressure;
float Altitude;

void setup() 
{
  Serial.begin(115200);
  Wire.begin();
  // Initialize MS5611 sensor
  // Ultra high resolution: MS5611_ULTRA_HIGH_RES
  // (default) High resolution: MS5611_HIGH_RES
  // Standard: MS5611_STANDARD
  // Low power: MS5611_LOW_POWER
  // Ultra low power: MS5611_ULTRA_LOW_POWER
  ms5611.begin( MS5611_ULTRA_HIGH_RES );

  // BME280 initialized with highest precision oversampling for pressure reading
  bme.begin();

  // INITIAL SETUP OF MAX AND MIN
  Pressure = ms5611.readPressure(false);
  Altitude = ms5611.getAltitudeFeet(Pressure, slp);
  maxMS5611u = minMS5611u = Altitude;

  Pressure = ms5611.readPressure(true);
  Altitude = ms5611.getAltitudeFeet(Pressure, slp);
  maxMS5611c = minMS5611c = Altitude;

  Altitude = bme.readFloatAltitudeFeet( slp );
  maxBME280 = minBME280 = Altitude;

  delay(5);

}

void loop()
{
	uint32_t timestamp = millis();

/**************  MS5611 **********************************/
  // without compensation
  Pressure = ms5611.readPressure(false);
  delay( 5 );
  Altitude = ms5611.getAltitudeFeet(Pressure, slp);

  if( Altitude > maxMS5611u) 
  {
	  maxMS5611u = Altitude;
  }
  if( Altitude < minMS5611u) 
  {
	  minMS5611u = Altitude;
  }

  Serial.print( "MS5611 Uncompensated altitude feet= " );
  Serial.print(Altitude);

  float msU = maxMS5611u - minMS5611u;
  Serial.print( "     MAX " ); Serial.print( maxMS5611u ); 
  Serial.print( " MIN " ); Serial.print( minMS5611u );  
  Serial.print( " Difference " );  Serial.println( msU );

  // with compensation
  Pressure = ms5611.readPressure(true);
  delay( 5 );
  Altitude = ms5611.getAltitudeFeet(Pressure, slp);

  if( Altitude > maxMS5611c) 
  {
	  maxMS5611c = Altitude;
  }
  if( Altitude < minMS5611c) 
  {
	  minMS5611c = Altitude;
  }

  Serial.print("MS5611 Compensated Altitude feet= ");
  Serial.print(Altitude);

  float msC = maxMS5611c - minMS5611c;
  Serial.print( "       MAX " ); Serial.print( maxMS5611c ); 
  Serial.print( " MIN " ); Serial.print( minMS5611c );  
  Serial.print( " Difference " );  Serial.println(msC );

/*****************  Pressure updated within the routine  **************/
  Altitude = bme.readFloatAltitudeFeet( slp );
  if( Altitude > maxBME280) 
  {
	  maxBME280 = Altitude;
  }
  if( Altitude < minBME280) 
  {
	  minBME280 = Altitude;
  }

  Serial.print("BME280 Compensated Altitude feet= ");
  Serial.print(Altitude);

  float bm = maxBME280 - minBME280;
  Serial.print( "       MAX " ); Serial.print( maxBME280 ); 
  Serial.print( " MIN " ); Serial.print( minBME280 );  
  Serial.print( " Difference " );  Serial.println( bm );

  // who is the winner?

  byte winner = 0; // nobody
  if ( bm < msC )
  {
	  if ( bm < msU )
	  {
		  winner = 1; // BME280
	  }
	  else if( msU < msC )
	  {
		  winner = 2; // MS5611u
	  }
  }
  else if(msC < msU )
  {
	  winner = 3; //MS5611c
  }
  else
  {
	  winner = 2; //MS5611u
  }
  Serial.print( "\n********* MILLION $$$ winner in consistency is ******" );
  switch(winner)
  {
	  case 1:
		  Serial.println( " BME280 ****" );
		  break;
	  case 2:
		  Serial.println( " MS5611 Uncompensated ******" );
		  break;
	  case 3:
		  Serial.println( " MS5611 Compensated ******" );
		  break;
	  default:
		  break;
  }

  Serial.println("\n");

  uint32_t loop = millis() - timestamp;
  delay( 5000 - loop ); // make loop exactly 5 sec
}

