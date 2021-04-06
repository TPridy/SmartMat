#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include "settings.h"

SoftwareSerial SmartMat(D5,D6);

int mat_mode = HOME;
int weight_mode = POUNDS;

/* Blynk */
#define BLYNK_PRINT Serial
BlynkTimer timer;
char auth[] = "tZ6PCOfcGv_y7vjUbSByng7nT2XGm4de";
char ssid[] = "ORBI";
char pass[] = "tommyp54";
//Attach virtual serial terminal to Virtual Pin V3
WidgetTerminal terminal(V3);

char inBuffer[64];



//****************************************************
//BLYNK                                              *
//****************************************************

BLYNK_WRITE(V2)
{
  if (param.asInt() == LOCKED) 
  {
    Serial.println("NodeMCU: Locked");
    mat_mode == LOCKED;
  } 
  else 
  {
    Serial.println("NodeMCU: Unlocked");
    mat_mode == NONE;
  }
}

BLYNK_WRITE(V0)
{
  char message[4];
  int blynk_mode = param.asInt();
  if (blynk_mode != mat_mode)
  {
    switch(blynk_mode)
    {
      case HOME:        Serial.println("NodeMCU: Changing mode to HOME...");
                        terminal.print("NodeMCU: Changing mode to HOME...\n");
                        mat_mode = HOME;
                        //Send mode change to SmartMat
                        message[0] = '{';
                        message[1] = CHANGE_MODE;
                        message[2] = HOME;
                        message[3] = '}';
                        SmartMat.write(message,4);
                        Serial.println("NodeMCU: Mode changed to HOME.");
                        terminal.print("NodeMCU: Mode changed to HOME.\n");
                        break;
      case AWAY:        Serial.println("NodeMCU: Changing mode to AWAY...");
                        terminal.print("NodeMCU: Changing mode to AWAY...\n");
                        mat_mode = AWAY;
                        //Send mode change to SmartMat
                        message[0] = '{';
                        message[1] = CHANGE_MODE;
                        message[2] = AWAY;
                        message[3] = '}';
                        SmartMat.write(message,4);
                        Serial.println("NodeMCU: Mode changed to AWAY.");
                        terminal.print("NodeMCU: Mode changed to AWAY.\n");
                        break;
      case NIGHT:       Serial.println("NodeMCU: Changing mode to NIGHT...");
                        terminal.print("NodeMCU: Changing mode to NIGHT...\n");
                        mat_mode = NIGHT;
                        //Send mode change to SmartMat
                        message[0] = '{';
                        message[1] = CHANGE_MODE;
                        message[2] = NIGHT;
                        message[3] = '}';
                        SmartMat.write(message,4);
                        Serial.println("NodeMCU: Mode changed to NIGHT.");
                        terminal.print("NodeMCU: Mode changed to NIGHT.\n");
                        break;
      case ALARM:       Serial.println("NodeMCU: Changing mode to ALARM...\n");
                        terminal.print("NodeMCU: Changing mode to ALARM...\n");
                        mat_mode = ALARM;
                        //Send mode change to SmartMat;
                        message[0] = '{';
                        message[1] = CHANGE_MODE;
                        message[2] = ALARM;
                        message[3] = '}';
                        SmartMat.write(message,4);
                        Serial.println("NodeMCU: Mode changed to ALARM.");
                        terminal.print("NodeMCU: Mode changed to ALARM.\n");
                        break;
      case LOCKED:      Serial.println("NodeMCU: Changing mode to LOCKED...");
                        terminal.print("NodeMCU: Changing mode to LOCKED...\n");
                        mat_mode = LOCKED;
                        //Send mode change to SmartMat
                        message[0] = '{';
                        message[1] = CHANGE_MODE;
                        message[2] = LOCKED;
                        message[3] = '}';
                        SmartMat.write(message,4);
                        Serial.println("NodeMCU: Mode changed to LOCKED.");
                        terminal.print("NodeMCU: Mode changed to LOCKED.\n");
                        break;
      default:          Serial.print("NodeMCU: Failed to change mode...\n");
                        terminal.print("NodeMCU: Failed to change mode...\n");
                        break;
    }
  }
}

BLYNK_WRITE(V1)
{
  char message[4];
  int blynk_weight_mode = param.asInt();
  if (blynk_weight_mode != weight_mode)
  {
    switch(blynk_weight_mode)
    {
      case KILOGRAMS:   Serial.println("NodeMCU: Changing weight mode to KILOGRAMS...");
                        terminal.print("NodeMCU: Changing weight mode to KILOGRAMS...\n");
                        weight_mode = KILOGRAMS;
                        //Send mode change to SmartMat
                        message[0] = '{';
                        message[1] = CHANGE_WEIGHT_MODE;
                        message[2] = KILOGRAMS;
                        message[3] = '}';
                        SmartMat.write(message,4);
                        Serial.println("NodeMCU: Weight Mode changed to KILOGRAMS.");;
                        terminal.print("NodeMCU: Weight Mode changed to KILOGRAMS.\n");
                        break;
      case POUNDS:      Serial.println("NodeMCU: Changing weight mode to POUNDS...");
                        terminal.print("NodeMCU: Changing weight mode to POUNDS...\n");
                        weight_mode = POUNDS;
                        //Send mode change to SmartMat
                        message[0] = '{';
                        message[1] = CHANGE_WEIGHT_MODE;
                        message[2] = POUNDS;
                        message[3] = '}';
                        SmartMat.write(message,4);
                        Serial.println("NodeMCU: Mode changed to POUNDS.");
                        terminal.print("NodeMCU: Mode changed to POUNDS.\n");
                        break;
      default:          Serial.println("NodeMCU: Failed to change mode...\n");
                        terminal.print("NodeMCU: Failed to change mode...\n");
                        break;
    }
  }
}

//****************************************************
//UART Communication                                 *
//****************************************************

void checkforMessage()
{ 
  if (SmartMat.available() > 0)
  {
    inBuffer[0] = SmartMat.read();
    byte done = OFF;
    byte i = 0;

    if(inBuffer[0] == '{')
    {
      Serial.println("NodeMCU: Received a Message from NodeMCU");
      delay(1000);
      while((done == OFF) && (SmartMat.available() > 0))
      {
        inBuffer[i+1] = SmartMat.read();
        if (inBuffer[i+1] == '}')
        {
          done = ON;
        }
        i = i+1;
      }
      /*for (byte t = 0; t < i+1; t++)
      {
        Serial.print(inBuffer[t]);
      }
      Serial.print("\n");*/
      decodeMessage(i);
    }
 
  }
}

void decodeMessage(char i)
{
    /*
    This is where the message received from the NodeMCU 
    will be decoded to determine what to do.
    */
    switch(inBuffer[1])
    {
      case SEND_NOTIFICATION:
        switch(inBuffer[2]) 
        {
          case 0:           Serial.println("Sending Notification...");
                            Blynk.notify("There has been a problem ");
                            break;
          default:          Serial.println("ERROR");
                            break;                                                                                
        }
        break;
      default:  
        Serial.print("ERROR: Cannot decode the message -> ");
        Serial.println(inBuffer);
        break;
    }
    return;
}

void setup() 
{
  //Start Serial Communications
  Serial.begin(115200); 
  SmartMat.begin(57600);
  
  //Start Up Message
  Serial.println("NodeMCU: Starting...");
  
  //Connect to SmartPhone Application
  Serial.println("NodeMCU: Establishing Network Connection...");
  Blynk.begin(auth, ssid, pass);

  //Clear Virtual Terminal
  terminal.clear(); 
  
  Serial.println("NodeMCU: Network Connection established.");
  terminal.println("NodeMCU: Connection established.");
  
  // Notify immediately on startup
  Blynk.notify("NodeMCU: SmartMat is online.");
  terminal.println("NodeMCU: SmartMat is online.");
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

  //Check for Message
  checkforMessage();
  
}
 
