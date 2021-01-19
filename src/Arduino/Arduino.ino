#include <HX711_ADC.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

SoftwareSerial ESPserial(2, 3); // RX | TX
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

 
void setup()
{
    Serial.begin(115200);     // communication with the host computer
    //while (!Serial)   { ; }
 
    // Start the software serial for communication with the ESP8266
    ESPserial.begin(115200); 
 
    Serial.println("");
    Serial.println("Ready");
    Serial.println("");   
}
 
void loop()
{
    // listen for communication from the ESP8266 and then write it to the serial monitor
    if ( ESPserial.available() )   {  
      //Serial.write( ESPserial.read() );
      Serial.println(ESPserial.read());
      }
 
    // listen for user input and send it to the ESP8266
    //if ( Serial.available() )       {  
      //ESPserial.write( Serial.read() );
      //Serial.println(Serial.read());
      //}
}
/*
void setup() {
  //Initialize the Load Cell Amplifier


}

void loop() {
  // put your main code here, to run repeatedly:

}*/
