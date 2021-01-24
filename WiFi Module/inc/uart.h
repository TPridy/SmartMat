/*
  uart.h - Library for uart to be used with SmartMat.
  Created by Thomas C Pridy, January 21st, 2021.
  Released into the public domain.
*/

#ifndef Uart_h
#define Uart_h
#include "Arduino.h"
#include "settings.h"

enum ControlMessages{
    CHANGE_MODE,
    CHANGE_WEIGHT_MODE
};

void initializeCommunications();
void decodeMessage(const char *message,char length,Settings *settings);

#endif
