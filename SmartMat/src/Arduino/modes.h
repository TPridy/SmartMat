/*
  modes.h - Library for modes to be used with SmartMat.
  Created by Thomas C Pridy, January 21st, 2021.
  Released into the public domain.
*/

#ifndef Modes_h
#define Modes_h
#include "Arduino.h"

void noneModeHandle();
void stayAtHomeModeHandle();
void awayModeHandle();
void nightModeHandle();
void lockedModeHandle();
void alarmModeHandle();

#endif