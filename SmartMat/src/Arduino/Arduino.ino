#include <SoftwareSerial.h>
#include "uart.h"
#include "settings.h"
#include "modes.h"
#include "pressure_sensors.h"


//Settings *settings = new Settings(STAYATHOME,POUNDS);
 
void setup()
{  
  //Start the software serial for communication with the ESP8266
  initializeCommunications();
  /*if (initializePressureSensors() == EXIT_FAILURE)
  {
    Serial.println("SmartMat: Pressure Sensors failed to initialize.");
  }*/
  Serial.println("SmartMat: Initialization Complete");
  Serial.print("\n");
  Serial.println("            SmartMat             ");
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

  //Check for message from NodeMCU
  checkforMessage(settings);
}
