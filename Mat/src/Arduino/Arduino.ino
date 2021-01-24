#include <SoftwareSerial.h>
#include "uart.h"
#include "settings.h"
#include "modes.h"

SoftwareSerial ESPserial(12, 13); // RX | TX
Settings *settings = new Settings(NONE,POUNDS);
char inputBuffer[16];
 
void setup()
{  
  //Start the software serial for communication with the ESP8266
  initializeCommunications();
  
  Serial.println("SmartMat: Initialization Complete");
  Serial.print("\n");
  Serial.println("*********************************");
  Serial.println("*          SmartMat             *");
  Serial.println("*********************************");
  Serial.print("Mode: ");
  switch(settings->getMode()) 
  {
    case NONE:        Serial.println("NONE");
                      break;
    case STAYATHOME:  Serial.println("STAYATHOME");
                      break;
    case AWAY:        Serial.println("AWAY");
                      break;
    case NIGHT:       Serial.println("NIGHT");
                      break;
    case LOCKED:      Serial.println("LOCKED");
                      break;
    case ALARM:       Serial.println("ALARM");
                      break;
    default:          Serial.println("\nERROR: Did not recognize mode...");
                      break;                                                                                
  }
  Serial.print("Weight Mode: ");
  switch(settings->getWeightMode()) 
  {
    case KILOGRAMS:   Serial.println("KILOGRAMS");
                      break;
    case POUNDS:      Serial.println("POUNDS");
                      break;
    default:          Serial.println("\nERROR: Did not recognize mode...");
                      break;                                                                                
  }
  Serial.print("\n");
}
 
void loop()
{
  switch(settings->getMode())
  {
    case NONE:        noneModeHandle();
                      break;
    case STAYATHOME:  stayAtHomeModeHandle();
                      break;
    case AWAY:        awayModeHandle();
                      break;
    case NIGHT:       nightModeHandle();
                      break;
    case LOCKED:      lockedModeHandle();
                      break;
    case ALARM:       alarmModeHandle();
                      break;
    default:          noneModeHandle(); 
                      break;
  }

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
