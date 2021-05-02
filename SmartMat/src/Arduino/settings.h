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

//Identifcation
#define PACKAGE 1
#define PERSON  2

//Messages
#define CHANGE_MODE 0
#define CHANGE_WEIGHT_MODE 1
#define CHANGE_NOTIFICATION_MODE 2
#define NOTIFICATIONS 3

// Notifications
#define SEND_NOTIFICATION 1
#define SEND_EMAIL 2
#define DONT_SEND 3
#define LIVE_STREAM_WEIGHT 4

//WiFi Status
#define DISCONNECTED 0
#define CONNECTED 1

//Flags
#define OFF 0
#define ON 1

//Pins
#define DATA    13
#define SCLK    12
#define BUZZER  10
#define RX      7
#define TX      8

//Weight Detection
//#define CALIBRATION_FACTOR -7050.0
#define CALIBRATION_FACTOR -11098
#define THRESHOLD 1

//Weight Distribution
/*#define s0  A4
#define s1  A3
#define s2  A2
#define s3  A1
#define w0  6
#define w1  5
#define w2  4
#define w3  3
#define OUT_pin  2
#define SIG_pin  A0*/

#endif
