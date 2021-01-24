/*
  pressure_sensors.h - Library for Pressure Sensors to be used with SmartMat.
  Created by Thomas C Pridy, January 21st, 2021.
  Released into the public domain.
*/

#ifndef Pressure_Sensors_h
#define Pressure_Sensors_h

#include "Arduino.h"

//Pins
#define DOUT 11
#define CLK  10

//Calibration Factor
#define CALIBRATION_FACTOR -7050.0

//Initialization Functions
int initializePressureSensors();

//Data Functions
float getWeight();
float getLastReading();

//Settings
void calibrateHX711(float calibration_factor);

#endif