/*
  uart.cpp - Library for uart to be used with SmartMat.
  Created by Thomas C Pridy, January 21st, 2021.
  Released into the public domain.
*/

#include <string.h>
#include "uart.h"
#include "Arduino.h"

void initializeCommunications()
{
  /*
  Start the serial communication from the main software running on the SmartMat
  and the NodeMCU which connects to the internet and the phone application. The 
  default baud rate will be 9600.
  */
  Serial.begin(9600); 
  Serial.print("Initializing Communications...\n");
}

void decodeMessage(const char *message)
{
    /*
    This is where the message received from the NodeMCU 
    will be decoded to determine what to do.
    */
    if (strcmp(message, "012345678") == 0)
    {
        Serial.print("Received!\n");
    }
    else
    {
        Serial.print("ERROR: Cannot decode the message -> ");
        Serial.println(message);
    }
    return;
}
