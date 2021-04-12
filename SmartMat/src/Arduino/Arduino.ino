
#include <SoftwareSerial.h>
#include "HX711.h"
#include <stdlib.h>
SoftwareSerial nodemcu(7, 8); // nodemcu module connected here
#define DOUT  13
#define CLK  12
HX711 scale;

 

String myString; 
String cmessage; // complete message
char buff[10];
float weight; 
int counter;
float calibration_factor = -11098; // for me this vlaue works just perfect -11098 

void setup() {
  Serial.begin(9600);
  nodemcu.begin(9600); 
  scale.begin(DOUT, CLK); 
  scale.set_scale();
  scale.tare(); // Reset the scale to 0
  long zero_factor = scale.read_average(); // Get a baseline reading
 
}

void loop() {
   scale.set_scale(calibration_factor); // Adjust to this calibration factor
   weight = scale.get_units(4); //4 --> refresh weight rate
   myString = dtostrf(weight, 3, 2, buff); // convert float weight to string
   //Serial.println(myString);
   nodemcu.println(myString+'\r');
   myString ="";
   ;
  
   if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 10;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 10;
  }
  


  }
