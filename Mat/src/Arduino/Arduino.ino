#include <HX711_ADC.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "uart.h"
#include "settings.h"
#include "modes.h"

//SoftwareSerial ESPserial(2, 3); // RX | TX
Settings *settings = new Settings(STAYATHOME,POUNDS);
char inputBuffer[16];
 
void setup()
{
  initializeCommunications();
  // Start the software serial for communication with the ESP8266
  //ESPserial.begin(9600); 
  Serial.println("SmartMat: Initialization Complete");
  Serial.print("\n");
  Serial.println("---------------------------------");
  Serial.println("-          SmartMat             -");
  Serial.println("---------------------------------");
  Serial.print("Mode: ");
  Serial.println(settings->getMode());
  Serial.print("Weight Mode: ");
  Serial.println(settings->getWeightMode());
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
    case ALARM:       lockedModeHandle();
                      break;
    default:          alarmModeHandle(); 
                      break;
  }

  //Check for Message and Decode if necessary
  if ( Serial.available()>= 2 )
  {
    Serial.println("SmartMat: Message Received");
      /*
      Read in the message of 10 bytes into the buffer
      */
      for (char i=0;i<=2;i++)
      {
        inputBuffer[i] = Serial.read();
        /*if (i == 2)
        {
          inputBuffer[i] = 0;
        }
        else
        {
          
        }*/
      }
      /*
      Decode the message when received.
      */
      decodeMessage(inputBuffer,0,settings);
  }
}
