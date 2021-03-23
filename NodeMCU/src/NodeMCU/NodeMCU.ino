#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include "settings.h"

SoftwareSerial SmartMat(D5,D6);

int Mode = HOME;
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
  //Start Debug Serial Communications
  Serial.begin(115200); 
  Blynk.virtualWrite(V3, "clr");  // Terminal Widget

  //Start SmartMat Serial Communication
  SmartMat.begin(57600);
  
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

//****************************************************
//BLYNK                                              *
//****************************************************

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
  char message[4];
  switch(param.asInt())
  {
    case HOME:        Serial.println("NodeMCU: Changing mode to HOME...");
                      Blynk.virtualWrite (V3, "NodeMCU: Changing mode to HOME...\n");
                      Mode = HOME;
                      //Send mode change to SmartMat
                      message[0] = '{';
                      message[1] = 0;
                      message[2] = HOME;
                      message[3] = '}';
                      SmartMat.write(message,4);
                      Serial.println("NodeMCU: Mode changed to HOME.");
                      Blynk.virtualWrite (V3, "NodeMCU: Mode changed to HOME.\n");
                      break;
    case AWAY:        Serial.println("NodeMCU: AWAY");
                      Blynk.virtualWrite (V3, "NodeMCU: Changing mode to AWAY...\n");
                      Mode = AWAY;
                      //Send mode change to SmartMat
                      message[0] = '{';
                      message[1] = 0;
                      message[2] = AWAY;
                      message[3] = '}';
                      SmartMat.write(message,4);
                      Serial.println("NodeMCU: Mode changed to AWAY.");
                      Blynk.virtualWrite (V3, "NodeMCU: Mode changed to AWAY.\n");
                      break;
    case NIGHT:       Serial.println("NodeMCU: NIGHT");
                      Blynk.virtualWrite (V3, "NodeMCU: Changing mode to NIGHT...\n");
                      Mode = NIGHT;
                      //Send mode change to SmartMat
                      message[0] = '{';
                      message[1] = 0;
                      message[2] = NIGHT;
                      message[3] = '}';
                      SmartMat.write(message,4);
                      Serial.println("NodeMCU: Mode changed to NIGHT.");
                      Blynk.virtualWrite (V3, "NodeMCU: Mode changed to NIGHT.\n");
                      break;
    case ALARM:       Serial.println("NodeMCU: ALARM");
                      Blynk.virtualWrite (V3, "NodeMCU: Changing mode to ALARM...\n");
                      Mode = ALARM;
                      //Send mode change to SmartMat;
                      message[0] = '{';
                      message[1] = 0;
                      message[2] = ALARM;
                      message[3] = '}';
                      SmartMat.write(message,4);
                      Serial.println("NodeMCU: Mode changed to ALARM.");
                      Blynk.virtualWrite (V3, "NodeMCU: Mode changed to ALARM.\n");
                      break;
    case LOCKED:      Serial.println("NodeMCU: LOCKED");
                      Blynk.virtualWrite (V3, "NodeMCU: Changing mode to LOCKED...\n");
                      Mode = LOCKED;
                      //Send mode change to SmartMat
                      message[0] = '{';
                      message[1] = 0;
                      message[2] = LOCKED;
                      message[3] = '}';
                      SmartMat.write(message,4);
                      Serial.println("NodeMCU: Mode changed to LOCKED.");
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
 
