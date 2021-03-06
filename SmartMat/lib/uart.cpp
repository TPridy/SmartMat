/*
  uart.cpp - Library for uart to be used with SmartMat.
  Created by Thomas C Pridy, January 21st, 2021.
  Released into the public domain.
*/

#include <string.h>
#include <SoftwareSerial.h>
#include "uart.h"
#include "settings.h"
#include "Arduino.h"

SoftwareSerial ESPserial(12, 13); // RX | TX
char inputBuffer[16];

void initializeCommunications()
{
  /*
  Start the serial communication from the main software running on the SmartMat
  and the NodeMCU which connects to the internet and the phone application. The 
  default baud rate will be 9600.
  */
  Serial.begin(9600); 
  ESPserial.begin(9600); 
  Serial.print("SmartMat: Initializing Communications...\n");
}

void checkforMessage(Settings *settings)
{
  //Check for Message and Decode if necessary
  if ( ESPserial.available()>= 2 )
  {
    Serial.println("SmartMat: Message Received");
      /*
      Read in the message of 10 bytes into the buffer
      */
      for (char i=0;i<=2;i++)
      {
        inputBuffer[i] = ESPserial.read();
      }
      /*
      Decode the message when received.
      */
      decodeMessage(inputBuffer,0,settings);
  }
}

void decodeMessage(const char *message,char length,Settings *settings)
{
    /*
    This is where the message received from the NodeMCU 
    will be decoded to determine what to do.
    */
    switch(message[0])
    {
      case CHANGE_MODE:
        switch(message[1]) 
        {
          case NONE:        Serial.println("SmartMat: Changing mode to NONE...");
                            settings->setMode(NONE);
                            break;
          case STAYATHOME:  Serial.println("SmartMat: Changing mode to STAYATHOME...");
                            settings->setMode(STAYATHOME);
                            break;
          case AWAY:        Serial.println("SmartMat: Changing mode to AWAY...");
                            settings->setMode(AWAY);
                            break;
          case NIGHT:       Serial.println("SmartMat: Changing mode to NIGHT...");
                            settings->setMode(NIGHT);
                            break;
          case LOCKED:      Serial.println("SmartMat: Changing mode to LOCKED...");
                            settings->setMode(LOCKED);
                            break;
          case ALARM:       Serial.println("SmartMat: Changing mode to ALARM...");
                            settings->setMode(ALARM);
                            break;
          default:          Serial.println("ERROR: Did not recognize mode to change to...");
                            break;                                                                                
        }
        break;
      case CHANGE_WEIGHT_MODE:
        switch(message[1])
        {
          case KILOGRAMS:   Serial.println("SmartMat: Changing weight mode to KILOGRAMS...");
                            settings->setWeightMode(KILOGRAMS);
                            break;
          case POUNDS:      Serial.println("SmartMat: Changing weight mode to POUNDS...");
                            settings->setWeightMode(POUNDS);
                            break;
          default:          Serial.println("ERROR: Did not recognize weight mode to change to...");
                            break;                                                                                
        }
        break;
      default:  
        Serial.print("ERROR: Cannot decode the message -> ");
        Serial.println(message);
        break;
    }
    return;
}
