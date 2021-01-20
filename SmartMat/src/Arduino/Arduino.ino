#include <HX711_ADC.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "settings.h"


Settings settings(STAYATHOME,POUNDS);
SoftwareSerial ESPserial(2, 3); // RX | TX
char inChar=-1; // Where to store the character read

 
void setup()
{
    //Start the serial communication with the host computer (Include for debugging)
    Serial.begin(9600); 
    // Start the software serial for communication with the ESP8266
    ESPserial.begin(9600); 
 
}
 
void loop()
{
  switch(settings.getMode())
  {
    case NONE:        Serial.print("NONE\n");
                      break;
    case STAYATHOME:  Serial.print("STAYATHOME\n");
                      break;
    case AWAY:        Serial.print("AWAY\n");
                      break;
    case NIGHT:       Serial.print("NIGHT\n");
                      break;
    case LOCKED:      Serial.print("LOCKED\n");
                      break;
    default:          Serial.print("Something went wrong...\n");  
  }
    
    // listen for communication from the ESP8266 and then write it to the serial monitor
    /*if ( ESPserial.available() )   {  
      String content = "";
      //Serial.write( ESPserial.read() );
      Serial.println(content.concat(ESPserial.read()));
      }
 
    // listen for user input and send it to the ESP8266
    if ( Serial.available()>5 )       {  
      inChar = Serial.read();
      //ESPserial.write( Serial.read() );
      Serial.print(inChar);
      Serial.write(inChar);
      
      }*/
}


/*
/////////////////////////////////////////////////////////////////
// HX711 Load Amplifier Definitions
/////////////////////////////////////////////////////////////////
//pins:
const int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_sck = 5; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;
/////////////////////////////////////////////////////////////////
*/
