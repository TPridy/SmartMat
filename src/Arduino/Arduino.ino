#include <HX711_ADC.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

SoftwareSerial ESPserial(2, 3); // RX | TX

char inData[20]; // Allocate some space for the string
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character
char receiveFlag = 0;

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
    Serial.begin(9600);     // communication with the host computer
 
    // Start the software serial for communication with the ESP8266
    ESPserial.begin(9600); 
 
    Serial.println("");
    Serial.println("Ready");
    Serial.println("");   
}
 
void loop()
{

   /* if (ESPserial.available() >= 19) // Don't read unless
    {
        while(index < 19) // One less than the size of the array
        {
            inChar = ESPserial.read(); // Read a character
            inData[index] = inChar; // Store it
            index++; // Increment where to write next
            inData[index] = '\0'; // Null terminate the string
        }
        receiveFlag = 1;
    }
    if (receiveFlag){
      Serial.println(inData);
      for (int i=0;i<19;i++) {
        inData[i]=0;
      }
        index=0;
        receiveFlag = 0;
        Serial.println(inData);
    }*/
    
    // listen for communication from the ESP8266 and then write it to the serial monitor
    /*if ( ESPserial.available() )   {  
      String content = "";
      //Serial.write( ESPserial.read() );
      Serial.println(content.concat(ESPserial.read()));
      }*/
 
    // listen for user input and send it to the ESP8266
    if ( ESPserial.available()>5 )       {  
      inChar = ESPserial.read();
      //ESPserial.write( Serial.read() );
      Serial.print(inChar);
      }
}
