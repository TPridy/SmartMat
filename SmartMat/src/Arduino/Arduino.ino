#include <HX711_ADC.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "uart.h"
#include "settings.h"
#include "modes.h"

//SoftwareSerial ESPserial(2, 3); // RX | TX
Settings settings(STAYATHOME,POUNDS);
char inputBuffer[11];
 
void setup()
{
  initializeCommunications();
  // Start the software serial for communication with the ESP8266
  //ESPserial.begin(9600); 
}
 
void loop()
{
  switch(settings.getMode())
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
  if ( Serial.available()==10 )
  {
      /*
      Read in the message of 10 bytes into the buffer
      */
      for (char i=0;i<=10;i++)
      {
        if (i == 10)
        {
          inputBuffer[i] = 0;
        }
        else
        {
          inputBuffer[i] = Serial.read();
        }
      }
      /*
      Decode the message when received.
      */
      decodeMessage(inputBuffer);
  }
 
}
