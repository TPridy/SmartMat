/*
  pressure_sensors.cpp - Library for Pressure Sensors to be used with SmartMat.
  Created by Thomas C Pridy, January 21st, 2021.
  Released into the public domain.
*/

#include "pressure_sensors.h"
#include "Arduino.h"
#include "settings.h"
#include "HX711.h"

//HX711 constructor:
HX711  scale;

//Variables
float last_reading = 0;

int initializePressureSensors()
{
    //Print to Serial
    Serial.println("SmartMat: Initializing Pressure Sensors...");

    //Start up Scale
    scale.begin(DATA, SCLK);
    scale.set_scale(CALIBRATION_FACTOR);
    scale.tare();
}

float getWeight()
{
    //1 second delay for Last Reading
    delay(1000);
    
    //Get and print data
    Serial.print("SmartMat: Weight Data -> ");
    float reading = scale.get_units();
    Serial.println(reading,DEC);
    last_reading = reading;
    
    //Return weight
    return reading;
}

float getLastReading()
{
    //Return weight
    return last_reading;
}

void calibrateHX711(float calibration_factor)
{
    scale.set_scale(calibration_factor);
}
