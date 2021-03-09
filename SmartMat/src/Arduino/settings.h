/*
  settings.h - Library for settings to be used with SmartMat.
  Created by Thomas C Pridy, January 21st, 2021.
  Released into the public domain.
*/

#ifndef Settings_h
#define Settings_h
#include "Arduino.h"

//Modes
#define NONE 0
#define HOME 1
#define AWAY 2
#define NIGHT 3
#define LOCKED 4
#define ALARM 5

//Weight Modes
#define KILOGRAMS 1
#define POUNDS 2

//WiFi Status
#define DISCONNECTED 0
#define CONNECTED 1

//Pins
#define DATA 11
#define SCLK  10

//Calibration Factor
#define CALIBRATION_FACTOR -7050.0

//Threshold Value
#define THRESHOLD 100

#endif
