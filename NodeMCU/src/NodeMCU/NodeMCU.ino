#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include "settings.h"

SoftwareSerial SmartMat(D5,D6);

int mat_mode = HOME;
int last_mat_mode = HOME;
int weight_mode = POUNDS;
int notifications = DONT_SEND;

/* Blynk */
#define BLYNK_PRINT Serial
BlynkTimer timer;
char auth[] = "tZ6PCOfcGv_y7vjUbSByng7nT2XGm4de";
char ssid[] = "ORBI";
char pass[] = "tommyp54";
//Attach virtual serial terminal to Virtual Pin V3
//WidgetTerminal terminal(V3);

//UART Communication
char inBuffer[64];
int buffer_len = 0;
byte uart_flag = 0;


//****************************************************
//BLYNK                                              *
//****************************************************

BLYNK_WRITE(V2)
{
  char payload[4];
  if (param.asInt() == LOCKED) 
  {
    Serial.println("NodeMCU: Locked");
    last_mat_mode = mat_mode;
    mat_mode = LOCKED;
    //Send mode change to SmartMat
    payload[0] = '{';
    payload[1] = CHANGE_MODE;
    payload[2] = LOCKED;
    payload[3] = '}';
    SmartMat.write(payload,4);
  } 
  else 
  {
    Serial.println("NodeMCU: Unlocked");
    mat_mode = last_mat_mode;
    //Send mode change to SmartMat
    payload[0] = '{';
    payload[1] = CHANGE_MODE;
    payload[2] = mat_mode;
    payload[3] = '}';
    SmartMat.write(payload,4);
  }
}

BLYNK_WRITE(V0)
{
  char payload[4];
  int blynk_mode = param.asInt();
  if (blynk_mode != mat_mode)
  {
    //Turn off lock
    Blynk.virtualWrite(V2, 0);
    switch(blynk_mode)
    {
      case HOME:        Serial.println("NodeMCU: Changing mode to HOME...");
                        mat_mode = HOME;
                        //Send mode change to SmartMat
                        payload[0] = '{';
                        payload[1] = CHANGE_MODE;
                        payload[2] = HOME;
                        payload[3] = '}';
                        SmartMat.write(payload,4);
                        break;
      case AWAY:        Serial.println("NodeMCU: Changing mode to AWAY...");
                        mat_mode = AWAY;
                        //Send mode change to SmartMat
                        payload[0] = '{';
                        payload[1] = CHANGE_MODE;
                        payload[2] = AWAY;
                        payload[3] = '}';
                        SmartMat.write(payload,4);
                        break;
      case NIGHT:       Serial.println("NodeMCU: Changing mode to NIGHT...");
                        mat_mode = NIGHT;
                        //Send mode change to SmartMat
                        payload[0] = '{';
                        payload[1] = CHANGE_MODE;
                        payload[2] = NIGHT;
                        payload[3] = '}';
                        SmartMat.write(payload,4);
                        break;
      default:          Serial.print("NodeMCU: Failed to change mode...\n");
                        break;
    }
  }
}

BLYNK_WRITE(V1)
{
  char payload[4];
  int blynk_weight_mode = param.asInt();
  if (blynk_weight_mode != weight_mode)
  {
    switch(blynk_weight_mode)
    {
      case KILOGRAMS:   Serial.println("NodeMCU: Changing weight mode to KILOGRAMS...");
                        weight_mode = KILOGRAMS;
                        //Send mode change to SmartMat
                        payload[0] = '{';
                        payload[1] = CHANGE_WEIGHT_MODE;
                        payload[2] = KILOGRAMS;
                        payload[3] = '}';
                        SmartMat.write(payload,4);
                        break;
      case POUNDS:      Serial.println("NodeMCU: Changing weight mode to POUNDS...");
                        weight_mode = POUNDS;
                        //Send mode change to SmartMat
                        payload[0] = '{';
                        payload[1] = CHANGE_WEIGHT_MODE;
                        payload[2] = POUNDS;
                        payload[3] = '}';
                        SmartMat.write(payload,4);
                        break;
      default:          Serial.println("NodeMCU: Failed to change mode...\n");
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
    byte inByte = SmartMat.read();
    if (inByte == '{')
    {
      uart_flag = 1;
    }
    else if (inByte == '}')
    {
      uart_flag = 2;
    }
    else
    {
      inBuffer[buffer_len] = inByte;
      buffer_len++;
    }
  }
}


void decodeMessage()
{
  long weight = 0;
                                
    /*
    This is where the message received from the SmartMat 
    will be decoded to determine what to do.
    */
    switch(inBuffer[0])
    {
      case NOTIFICATIONS:
        
        switch(inBuffer[1]) 
        {
          case ALARM:                        Blynk.notify("The alarm on your SmartMat has been triggered!");
                                             Blynk.email("thomas.pridy@gmail.com","SmartMat Incident","The alarm on your SmartMat has been triggered!");
                                             Serial.println("NodeMCU: The alarm on your SmartMat has been triggered!");
                                             break;
          case DONT_SEND:                    Serial.print("NodeMCU: Received Notifcation '");
                                             Serial.print("There is something of weight " + String(weight) + "lb(s) on the SmartMat.");
                                             Serial.println("' but notifications are blocked.");
                                             break;
          case LIVE_STREAM_WEIGHT:           Serial.println("NodeMCU: Live Stream Weight is Dislayed on App...");
                                             weight |= inBuffer[2] << 24;
                                             weight |= inBuffer[3] << 16;
                                             weight |= inBuffer[4] << 8;
                                             weight |= inBuffer[5] << 0;
                                             Blynk.virtualWrite(V4, weight);
                                             break;
          case SEND_NOTIFICATION:            Serial.println("NodeMCU: Sending a Text Notification...");
                                             weight |= inBuffer[3] << 24;
                                             weight |= inBuffer[4] << 16;
                                             weight |= inBuffer[5] << 8;
                                             weight |= inBuffer[6] << 0;
                                             switch(inBuffer[2])
                                             {
                                              case NONE:    if (weight_mode == POUNDS)
                                                            {
                                                              Blynk.notify("There is something of weight " + String(weight) + "lb(s) on the SmartMat.");
                                                            }
                                                            else if (weight_mode == KILOGRAMS)
                                                            {
                                                              Blynk.notify("There is something of weight " + String(weight) + "kg(s) on the SmartMat.");
                                                            }                                                       
                                                            break;
                                              case PACKAGE: if (weight_mode == POUNDS)
                                                            {
                                                              Blynk.notify("There is a package of weight " + String(weight) + "lb(s) on the SmartMat.");
                                                            }
                                                            else if (weight_mode == KILOGRAMS)
                                                            {
                                                              Blynk.notify("There is a package of weight " + String(weight) + "kg(s) on the SmartMat.");
                                                            }                                                       
                                                            break;
                                              case PERSON:  if (weight_mode == POUNDS)
                                                            {
                                                              Blynk.notify("There is a person of weight " + String(weight) + "lb(s) on the SmartMat.");
                                                            }
                                                            else if (weight_mode == KILOGRAMS)
                                                            {
                                                              Blynk.notify("There is a person of weight " + String(weight) + "kg(s) on the SmartMat.");
                                                            }                                                       
                                                            break;
                                              default:      Serial.println("NodeMCU: Error in deciphering message");
                                                            break;
                                             }
                                             break;
          case SEND_EMAIL:                   switch(inBuffer[3])
                                             {
                                                case NONE:      if (weight_mode == POUNDS)
                                                                {
                                                                  Blynk.email("thomas.pridy@gmail.com","SmartMat Alert","There is something of weight " + String(weight) + "lb(s) on the SmartMat.");
                                                                }
                                                                else if (weight_mode == KILOGRAMS)
                                                                {
                                                                  Blynk.email("thomas.pridy@gmail.com","SmartMat Alert","There is something of weight " + String(weight) + "lb(s) on the SmartMat.");
                                                                } 
                                                                break;  
                                                case PACKAGE:   if (weight_mode == POUNDS)
                                                                {
                                                                  Blynk.email("thomas.pridy@gmail.com","SmartMat Alert","There is a package of weight " + String(weight) + "lb(s) on the SmartMat.");
                                                                }
                                                                else if (weight_mode == KILOGRAMS)
                                                                {
                                                                  Blynk.email("thomas.pridy@gmail.com","SmartMat Alert","There is a package of weight " + String(weight) + "lb(s) on the SmartMat.");
                                                                } 
                                                                break;  
                                                case PERSON:    if (weight_mode == POUNDS)
                                                                {
                                                                  Blynk.email("thomas.pridy@gmail.com","SmartMat Alert","There is a person of weight " + String(weight) + "lb(s) on the SmartMat.");
                                                                }
                                                                else if (weight_mode == KILOGRAMS)
                                                                {
                                                                  Blynk.email("thomas.pridy@gmail.com","SmartMat Alert","There is a person of weight " + String(weight) + "lb(s) on the SmartMat.");
                                                                } 
                                                                break;  
                                                default:        Serial.println("NodeMCU: Error in deciphering message");
                                                                break;
                                             }
                                             break;                                                                      
        }
        break;
      default:  
        Serial.print("ERROR: Cannot decode the message -> ");
        for (byte q = 0; q < buffer_len; q++)
        {
          Serial.print(inBuffer[q],HEX);
        }
        Serial.println("");
        break;
    }
    buffer_len = 0;
    uart_flag = 0;
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
  Serial.println("NodeMCU: Network Connection established.");
  
  // Notify immediately on startup
  Blynk.notify("NodeMCU: SmartMat is online.");

  //Set Start Values
  Blynk.virtualWrite(V0, mat_mode);
  Blynk.virtualWrite(V1, weight_mode);

  Serial.println("NodeMCU: NodeMCU Initialization Complete");
  Serial.print("\n");
  Serial.println("            NodeMCU             ");
  Serial.println("*********************************");
  Serial.print("Mode: ");
  switch(mat_mode) 
  {
    case NONE:        Serial.println("NONE");
                      break;
    case HOME:        Serial.println("HOME");
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
  switch(weight_mode) 
  {
    case KILOGRAMS:   Serial.println("KILOGRAMS");
                      break;
    case POUNDS:      Serial.println("POUNDS");
                      break;
    default:          Serial.println("\nERROR: Did not recognize mode...");
                      break;                                                                                
  }
  Serial.print("Notifications Mode: ");
  switch(notifications) 
  {
    case SEND_NOTIFICATION: Serial.println("TEXT");
                            break;
    case SEND_EMAIL:        Serial.println("EMAIL");
                            break; 
    case DONT_SEND:         Serial.println("NO MESSAGE");
                            break;                                                                 
    default:                Serial.println("\nERROR: Did not recognize mode...");
                            break;                                                                                
  }
  Serial.print("\n");
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
  if (uart_flag == 2)
  {
    decodeMessage();
  }
  
}
 
