
//NODEMCU

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include <SimpleTimer.h>
 
WidgetLCD lcd(V4);
WidgetLCD lcd1(V5);
SoftwareSerial SmartMat(D5,D6);
WidgetTerminal terminal(V3);
BlynkTimer timer;

//Modes
#define NONE 0
#define HOME 1
#define AWAY 2
#define VACATION 3

//Alarm
#define LOCKED 0
#define UNLOCKED 1

//Weight Modes
#define KILOGRAMS 1
#define POUNDS 2

//Identifcation
#define PACKAGE 1
#define PERSON  2

//Messages
#define CHANGE_MODE 0
#define CHANGE_WEIGHT_MODE 1
#define NOTIFICATIONS 3

// Notifications
#define SEND_NOTIFICATION 0
#define SEND_EMAIL 1
#define DONT_SEND 2
#define LIVE_STREAM_WEIGHT 3 

//Buzzer
#define BUZZER  5

#define BLYNK_PRINT Serial

char auth[] = "oi1FY_j2VLxRs_1XXWQ_1TVL0tn9hvwG";
 
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Delgado Family";
char pass[] = "windynest821";

//Mode Control
int mat_mode;
int weight_mode;
int locked_mode;
int blynk_weight_mode;
int notifications;

//Establish Notification message and email name
String message;
String emailname= "";

//Transferable Data
volatile byte indx;
char buff [50];

String weight;

// Units from Kilograms and Pounds Mode
String units;
float n;

//New Weight Conversion
float new_weight;  
int notified;

// This function sends Arduino's up time every second to Virtual Pin (1).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.

//****************************************************
//BLYNK                                              *
//****************************************************

BLYNK_WRITE(V2)
{
  int l_mode = param.asInt();
  switch(l_mode){
    case LOCKED:    Serial.println("NodeMCU: Changing mode to LOCKED...");
                    locked_mode == UNLOCKED;
    case UNLOCKED:  Serial.println("NodeMCU: Changing mode to UNLOCKED...");
                    locked_mode == UNLOCKED;
  }
}

BLYNK_WRITE(V6)
{
  Serial.println("NodeMCU: Changing email...");
  emailname = param.asStr();
  Serial.println("NodeMCU: Email changed to " + emailname);
  
}

BLYNK_WRITE(V0)
{
  int blynk_mode = param.asInt();
  if (blynk_mode != mat_mode)
  {
    switch(blynk_mode)
    {
      case HOME:        Serial.println("NodeMCU: Changing mode to HOME...");
                        terminal.print("NodeMCU: Changing mode to HOME...\n");
                        mat_mode = HOME;                      
                        Serial.println("NodeMCU: Mode changed to HOME.");
                        terminal.print("NodeMCU: Mode changed to HOME.\n");
                        break;
      case AWAY:        Serial.println("NodeMCU: Changing mode to AWAY...");
                        terminal.print("NodeMCU: Changing mode to AWAY...\n");
                        mat_mode = AWAY;
                        Serial.println("NodeMCU: Mode changed to AWAY.");
                        terminal.print("NodeMCU: Mode changed to AWAY.\n");
                        break;
      case VACATION:       Serial.println("NodeMCU: Changing mode to VACATION...");
                        terminal.print("NodeMCU: Changing mode to VACATION..\n");
                        mat_mode = VACATION;
                     
                        Serial.println("NodeMCU: Mode changed to VACATION.");
                        terminal.print("NodeMCU: Mode changed to VACATION.\n");
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
                      
                        Serial.println("NodeMCU: Weight Mode changed to KILOGRAMS.");;
                        terminal.print("NodeMCU: Weight Mode changed to KILOGRAMS.\n");
                        break;
      case POUNDS:      Serial.println("NodeMCU: Changing weight mode to POUNDS...");
                        terminal.print("NodeMCU: Changing weight mode to POUNDS...\n");
                        weight_mode = POUNDS;
                       
                        Serial.println("NodeMCU: Mode changed to POUNDS.");
                        terminal.print("NodeMCU: Mode changed to POUNDS.\n");
                        break;
      default:          Serial.println("NodeMCU: Failed to change mode...\n");
                        terminal.print("NodeMCU: Failed to change mode...\n");
                        break;
    }
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  SmartMat.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(BUZZER, OUTPUT);   // Buzzer alaram as output 
  digitalWrite (BUZZER, LOW);// Initially buzzer off   
}
 
void loop()
{ 
if (SmartMat.available() > 0) {
    byte c = SmartMat.read();
    if (indx < sizeof(buff)) {
      buff [indx++] = c; // save data in the next index in the array buff
      if (c == '\r') { //check for the end of the word
        decodeMessage(buff);
        indx= 0; //reset button to zero
      }
    }
  }
  if (Serial.available() == 0 ) 
   {
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
   }
} 
//  if (Serial.available() > 0 ) 
//  {
//    rdata = Serial.read();
//    myString = myString + rdata;
//    if( rdata == '\n')
//    {
//     // Serial.println(myString);
//      weight = getValue(myString, ',', 0);
// 
//      myString = "";
//    }
//  }



void decodeMessage(char i[50])
{
    notified = 0;
    n = atof (i);
    new_weight = n*(-1.0);
    Serial.println(new_weight);
    if (weight_mode == KILOGRAMS){
        Serial.println("Kilograms");
        new_weight = new_weight/2.205;
        units = "kg";
         }
     else if(weight_mode == POUNDS){
        Serial.println("Pounds");
        units = "lbs";
     }
    
    if( mat_mode == AWAY){
      if((new_weight >= 0.8) and (new_weight <= 50.0) and notified == 0 ){
        Serial.println("Package!");
        message= "There is a package of weight: "+ String(new_weight) + units;
        Blynk.notify(message);
        notified == 1;
        if(locked_mode == LOCKED){
            digitalWrite (BUZZER, HIGH);    //If intrusion detected ring the buzzer 
          }
          else{
            digitalWrite (BUZZER, LOW);
          }
        }
       else if((new_weight >= 51.0) and (new_weight < 400.0) and notified == 0 ){
            Serial.println("Human!"); 
            message = "There is a human of weight: " + String(new_weight)+ units;
            Blynk.notify(message);
             notified == 1;
          if(locked_mode == LOCKED){
            digitalWrite (BUZZER, HIGH);    //If intrusion detected ring the buzzer
          }
          else{
            digitalWrite (BUZZER, LOW);
          }
           
        }
        else{
           digitalWrite (BUZZER, LOW);    //If intrusion detected ring the buzzer
           message = "Your SmartMat is all clear! Have a nice day!";
           Blynk.notify(message);
           notified == 1;
        }
    }
    else if( mat_mode == HOME){
      lcd.print(0,0,"DETECTED:");
      lcd.print(0,1,String(new_weight)+ units);
      lcd.clear();
      lcd1.print(0,0,"STATUS:");   
      if((new_weight >= 0.8) and (new_weight <= 50.0)){
          Serial.println("Package!");
          lcd1.print(0,1,"PACKAGE");
          lcd1.clear();
          
        }
       else if(new_weight >= 51.0 and new_weight < 400.0){
          Serial.println("Human!");
          lcd1.print(0,1,"HUMAN");
          lcd1.clear();
        }
        else{
          lcd1.print(0,1,"ALL CLEAR");
          lcd1.clear();
        }
    }
    if( mat_mode == VACATION){   
      if((new_weight >= 0.29) and (new_weight <= 50.0) and notified == 0){
          Serial.println("Package!");
           // Serial.println(emailname.c_str());
           // String new_email = emailname.c_str();
            Blynk.email(emailname.c_str(), "SmartMat Alert", "There is a package on your SmartMat of weight: " + String(new_weight) + units);
            notified == 1;
        }
       else if(new_weight >= 51.0 and new_weight < 400.0 and notified == 0){
          Serial.println("Human!");
          Blynk.email(emailname.c_str(), "SmartMat Alert", "There is human on your SmartMat of weight: "+ String(new_weight) + units);            
          notified ==1;
            
        }
        else{
          Blynk.email(emailname.c_str(), "SmartMat Alert", "Your SmartMat is all clear! Have a nice day!");
          notified == 1;
        }
    }
   
}

 
