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
int Mode = STAYATHOME;
int WeightMode = POUNDS;

/* Blynk */
#define BLYNK_PRINT Serial
BlynkTimer timer;
char auth[] = "tZ6PCOfcGv_y7vjUbSByng7nT2XGm4de";
char ssid[] = "ORBI";
char pass[] = "tommyp54";
//Attach virtual serial terminal to Virtual Pin V1
WidgetTerminal terminal(V3);

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
  Blynk.virtualWrite(V3, "clr");  // Terminal Widget

  //Start Up Message
  Serial.println("NodeMCU: Starting...");
  Blynk.virtualWrite(V3, "NodeMCU: Starting...\n");

  //Connect to SmartPhone Application
  Serial.println("NodeMCU: Establishing Network Connection...");
  Blynk.virtualWrite(V3, "NodeMCU: Establishing Network Connection...\n");
  Blynk.begin(auth, ssid, pass); 
  Serial.println("NodeMCU: Network Connection established.");
  Blynk.virtualWrite(V3, "NodeMCU: Connection established.\n");
  
  // Notify immediately on startup
  Blynk.notify("NodeMCU: SmartMat is online.");
  Blynk.virtualWrite(V3, "NodeMCU: SmartMat is online\n");

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
    Blynk.virtualWrite(V3, "NodeMCU: Reconnecting...\n"); 
    if (Blynk.connect() == CONNECTED)
    {
      Serial.println("NodeMCU: Failed to reconnect.");
      Blynk.virtualWrite(V3, "Failed to reconnect.\n");
    }
    else
    {
      Serial.println("NodeMCU: Reconnected!");
      Blynk.virtualWrite(V3, "NodeMCU: Reconnected!\n");
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

BLYNK_WRITE(V2)
{
  if (param.asInt() == LOCKED) 
  {
    Serial.println("NodeMCU: Locked");
    Mode == LOCKED;
  } 
  else 
  {
    Serial.println("NodeMCU: Unlocked");
    Mode == NONE;
  }
}

BLYNK_WRITE(V0)
{
  switch(param.asInt())
  {
    case STAYATHOME:  Serial.println("NodeMCU: Changing mode to STAYATHOME...");
                      Blynk.virtualWrite (V3, "NodeMCU: Changing mode to STAYATHOME...\n");
                      Mode = STAYATHOME;
                      Blynk.virtualWrite (V3, "NodeMCU: Mode changed to STAYATHOME.\n");
                      break;
    case AWAY:        Serial.println("NodeMCU: AWAY");
                      Blynk.virtualWrite (V3, "NodeMCU: Changing mode to AWAY...\n");
                      Mode = AWAY;
                      Blynk.virtualWrite (V3, "NodeMCU: Mode changed to AWAY.\n");
                      break;
    case NIGHT:       Serial.println("NodeMCU: NIGHT");
                      Blynk.virtualWrite (V3, "NodeMCU: Changing mode to NIGHT...\n");
                      Mode = NIGHT;
                      Blynk.virtualWrite (V3, "NodeMCU: Mode changed to NIGHT.\n");
                      break;
    case ALARM:       Serial.println("NodeMCU: ALARM");
                      Blynk.virtualWrite (V3, "NodeMCU: Changing mode to ALARM...\n");
                      Mode = ALARM;
                      Blynk.virtualWrite (V3, "NodeMCU: Mode changed to ALARM.\n");
                      break;
    case LOCKED:      Serial.println("NodeMCU: LOCKED");
                      Blynk.virtualWrite (V3, "NodeMCU: Changing mode to LOCKED...\n");
                      Mode = LOCKED;
                      Blynk.virtualWrite (V3, "NodeMCU: Mode changed to LOCKED.\n");
                      break;
    default:          Serial.print("NodeMCU: UNABLE TO DETECT");
                      Blynk.virtualWrite (V3, "NodeMCU: Failed to change mode...\n");
                      break;
  }
}

BLYNK_WRITE(V1)
{
  switch(param.asInt())
  {
    case KILOGRAMS:   Serial.println("NodeMCU: Changing weight mode to KILOGRAMS...");
                      Blynk.virtualWrite (V3, "NodeMCU: Changing weight mode to KILOGRAMS...\n");
                      WeightMode = KILOGRAMS;
                      Blynk.virtualWrite (V3, "NodeMCU: Mode changed to KILOGRAMS.\n");
                      break;
    case POUNDS:      Serial.println("NodeMCU: Changing weight mode to POUNDS...");
                      Blynk.virtualWrite (V3, "NodeMCU: Changing weight mode to POUNDS...\n");
                      WeightMode = POUNDS;
                      Blynk.virtualWrite (V3, "NodeMCU: Mode changed to POUNDS.\n");
                      break;
    default:          Serial.println("NodeMCU: UNABLE TO DETECT");
                      Blynk.virtualWrite (V3, "NodeMCU: Failed to change weight mode...\n");
                      break;
  }
}
 
