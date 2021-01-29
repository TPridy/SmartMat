#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include "settings.h"
#include "modes.h"
#include "uart.h"
//SoftwareSerial s(D6,D5);

//char message[2];
//char message2[2];
//char message3[2];
//int data;
int lock = 0;

/* Blynk */
#define BLYNK_PRINT Serial
BlynkTimer timer;
char auth[] = "tZ6PCOfcGv_y7vjUbSByng7nT2XGm4de";
char ssid[] = "ORBI";
char pass[] = "tommyp54";

void setup() 
{
  //s.begin(9600);
  //Serial.begin(9600);
  /*message[0] = CHANGE_MODE;
  message[1] = AWAY;
  message2[0] = CHANGE_MODE;
  message2[1] = LOCKED;
  message3[0] = CHANGE_WEIGHT_MODE;
  message3[1] = 100;*/

  //Start Debug Console
  Serial.begin(9600); 

  //Start Up Message
  Serial.println("NodeMCU: Starting...");

  //Connect to SmartPhone Application
  Serial.println("NodeMCU: Establishing Network Connection...");
  Blynk.begin(auth, ssid, pass); 
  Serial.println("NodeMCU: Network Connection established.");
  
  // Notify immediately on startup
  Blynk.notify("NodeMCU: SmartMat is online");

  /*if (BLYNK_WRITE(V1) == 1)
  {
    Serial.println("NodeMCU: Server == LOCKED");
  }
  else
  {
    Serial.println("NodeMCU: Server == UNLOCKED");
  }*/

}
 
void loop() 
{

  //Check for connection Status
  if (Blynk.connected() == CONNECTED)
  {
    Blynk.run();
  }
  else
  {
    Serial.println("NodeMCU: Reconnecting...");
    if (Blynk.connect() == CONNECTED)
    {
      Serial.println("NodeMCU: Failed to reconnect.");
    }
    else
    {
      Serial.println("NodeMCU: Reconnected!");
    }
    delay(5000);
  }
  
  /*
  Serial.println("NodeMCU: Changing mode to AWAY");
  s.write(message,2);
  Serial.println(message);
  delay(1000);
  Serial.println("NodeMCU: Changing mode to LOCKED");
  s.write(message2,2);
  Serial.println(message2);
  delay(1000);
  Serial.println("NodeMCU: Changing weight mode to KILOGRAMS");
  s.write(message3,2);
  Serial.println(message3);
  delay(1000);
  //s.write("0123456789");
  //Serial.println("0123456789");*/
}

BLYNK_WRITE(V1)
{
  if (param.asInt() == LOCKED) 
  {
    Serial.println("NodeMCU: Locked");
    lock == LOCKED;
  } 
  else 
  {
    Serial.println("NodeMCU: Unlocked");
    lock == NONE;
  }
}

BLYNK_WRITE(V0)
{
  switch(param.asInt())
  {
    case STAYATHOME:  Serial.println("NodeMCU: STAYATHOME");
                      break;
    case AWAY:        Serial.println("NodeMCU: AWAY");
                      break;
    case NIGHT:       Serial.println("NodeMCU: NIGHT");
                      break;
    case ALARM:       Serial.println("NodeMCU: ALARM");
                      break;
    case LOCKED:      Serial.println("NodeMCU: LOCKED");
                      break;
    default:          Serial.println("NodeMCU: UNABLE TO DETECT");
                      break;
  }
}
 
