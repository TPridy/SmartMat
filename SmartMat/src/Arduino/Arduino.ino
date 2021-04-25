#include <SoftwareSerial.h>
#include "settings.h"
#include "HX711.h"
#include "SimpleTimer.h"

//Settings
byte volatile mode = HOME;
byte volatile weight_mode = POUNDS;
byte volatile notification_mode = SEND_NOTIFICATION;

//Flags
byte mode_flag = OFF;

//Accurate Weight Detection Variables
HX711  scale;
float weight = 0;

//Accurate Weight Distribution Variables    
int matrix[16][16];   //Value Matrix
int calibra[16][16];  //Calibration Matrix
int valor = 0;
int minsensor=254;
const boolean muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };

//UART Communication
SoftwareSerial NodeMCU(RX, TX); 
byte length_buffer = 0;
char inBuffer[32];
int buffer_len = 0;
byte uart_flag = 0;

//Timers
SimpleTimer receiveTimer(1000);
SimpleTimer liveStreamTimer(400);
SimpleTimer weightChangeTimer(1500);
SimpleTimer buzzerTimer(30000);

//****************************************************
//Secret Sauce                                       *
//****************************************************

void homeModeHandle()
{
  Serial.println("SmartMat: HOME Handle...");

  //Determine Package or Person
  int identifier = PACKAGE;

  //Notify Homeowner
  char message[9];
  long int reading = (long int) getWeight();
  if (reading < 1) reading = 0;
  else
  {
    message[0] = '{';
    message[1] = NOTIFICATIONS;
    message[2] = notification_mode;
    message[3] = identifier;
    message[4] = (reading) >> 24;
    message[5] = (reading) >> 16;
    message[6] = (reading ) >> 8;
    message[7] = (reading) >> 0;
    message[8] = '}';
  
    NodeMCU.write(message,9);
  }
}

void awayModeHandle()
{
  Serial.println("SmartMat: AWAY Handle...");
  
  //Serial.print("AWAY\n");
  /*
  if (getWeight() > THRESHOLD)
  {
    //Send Notifcation that there is weight on the mat
  }
  */
  return;
}

void nightModeHandle()
{
  Serial.println("SmartMat: NIGHT Handle...");
  
  //Serial.print("NIGHT\n");
  /*
  if (getWeight() > THRESHOLD)
  {
    //Send Notifcation that there is weight on the mat
  }
  */
  return;
}

void lockedModeHandle()
{
  Serial.println("SmartMat: LOCKED Handle...");
  
  alarmModeHandle();
  
  return;
}

void alarmModeHandle()
{
  Serial.println("SmartMat: ALARM activated.");
  
  //Activate buzzer
  digitalWrite(BUZZER, HIGH);
  buzzerTimer.reset();
  
  //Send notifications
  char message[9];
  long int reading = (long int) weight;
  message[0] = '{';
  message[1] = NOTIFICATIONS;
  message[2] = ALARM;
  message[3] = '}';

  NodeMCU.write(message,4);
}

//****************************************************
//Accurate Weight Detection                          *
//****************************************************

int initializeWeightDetection()
{
    //Print to Serial
    Serial.println("SmartMat: Initializing Accurate Weight Detection Layer...");

    //Start up Scale
    scale.begin(DATA, SCLK);
    scale.set_scale(CALIBRATION_FACTOR);
    scale.tare();

    //Return
    return EXIT_SUCCESS;
}

float getWeight()
{
    //Get data
    float reading = scale.get_units();
    
    //Return weight
    return reading;
}

void sendWeight()
{
  char message[5];
  long int reading = (long int) getWeight();
  if (reading < 0) reading = 0;
  
  message[0] = '{';
  message[1] = NOTIFICATIONS;
  message[2] = LIVE_STREAM_WEIGHT;
  message[3] = (reading) >> 24;
  message[4] = (reading) >> 16;
  message[5] = (reading ) >> 8;
  message[6] = (reading) >> 0;
  message[7] = '}';

  NodeMCU.write(message,8);
}

//****************************************************
//Accurate Weight Distribution                       *
//****************************************************

int initializeWeightDistribution()
{
    //Print to Serial
    Serial.println("SmartMat: Initializing Accurate Weight Distribution Layer...");

    //Initialize
    pinMode(s0, OUTPUT); 
    pinMode(s1, OUTPUT); 
    pinMode(s2, OUTPUT); 
    pinMode(s3, OUTPUT); 
    pinMode(w0, OUTPUT); 
    pinMode(w1, OUTPUT); 
    pinMode(w2, OUTPUT); 
    pinMode(w3, OUTPUT); 
    
    pinMode(OUT_pin, OUTPUT); 
   
    digitalWrite(s0, LOW);
    digitalWrite(s1, LOW);
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW);
    digitalWrite(w0, LOW);
    digitalWrite(w1, LOW);
    digitalWrite(w2, LOW);
    digitalWrite(w3, LOW);
    
    digitalWrite(OUT_pin, HIGH);
    
    //Initialize
    for(byte j = 0; j < 15; j ++)
    { 
      writeMux(j);
      for(byte i = 0; i < 15; i ++)
      {
        calibra[j][i] = 0;
      }
    }
    
    //Calibration
    for(byte k = 0; k < 50; k++)
    {  
      for(byte j = 0; j < 15; j ++)
      { 
        writeMux(j);
        for(byte i = 0; i < 15; i ++)
        {
          calibra[j][i] = calibra[j][i] + readMux(i);
        }
      }
    }

    //Print averages
    for(byte j = 0; j < 15; j ++)
    { 
      writeMux(j);
      for(byte i = 0; i < 15; i ++)
      {
        calibra[j][i] = calibra[j][i]/50;
        if(calibra[j][i] < minsensor) minsensor = calibra[j][i];
      }
    }
    
    //Return Success
    return EXIT_SUCCESS;
}


int readMux(byte channel)
{
  byte controlPin[] = {s0, s1, s2, s3};

  //loop through the 4 sig
  for(int i = 0; i < 4; i ++)
  {
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }

  //read the value at the SIG pin
  int val = analogRead(SIG_pin);

  //return the value
  return val;
}

void writeMux(byte channel)
{
  byte controlPin[] = {w0, w1, w2, w3};

  //loop through the 4 sig
  for(byte i = 0; i < 4; i ++)
  {
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }
}

void getAcuurateWeightDistributionMatrx()
{
  int t = 0;
    
  for(int j = 0; j < 15; j++)
  { 
    writeMux(j);
    for(int i = 0; i < 15; i++)
    {
      valor = readMux(i);
      
      //Saturation sensors
      int limsup = 450;
      if(valor > limsup) valor = limsup;
      if(valor < calibra[j][i] - 20) valor = calibra[j][i]; 
      valor = map(valor,minsensor, limsup,1,254); 
      
      if(valor <= 40) valor = 0;
      if(valor > 254) valor = 254;
     
      //Serial.write(valor);
      matrix[j][i] = valor;
    } 
  }
}

void printAccurateWeightDistributionMatrix()
{
  for(int j = 0; j < 15; j++)
      { 
        for(int i = 0; i < 15; i++)
        {
          Serial.print(matrix[j][i]);
          Serial.print("\t");
          if (i == 14)
          {
            Serial.print("\n");
          }
        } 
      }
      Serial.print("\n");
      Serial.print("\n");
      Serial.print("\n");
}

//****************************************************
//UART Communication                                 *
//****************************************************

void checkforMessage()
{ 
  if (NodeMCU.available() > 0)
  {
    byte inByte = NodeMCU.read();
    if (inByte == '{')
    {
      Serial.println("SmartMat: Incoming Message from NodeMCU");
      uart_flag = 1;
      receiveTimer.reset();
    }
    else if ((inByte == '}') && (uart_flag == 1))
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

void initializeCommunications()
{
  /*
  Start the serial communication from the main software running on the SmartMat
  and the NodeMCU which connects to the internet and the phone application. The 
  default baud rate will be 9600.
  */
  Serial.begin(115200); 
  NodeMCU.begin(57600); 
  pinMode(BUZZER, OUTPUT);
  Serial.println("SmartMat: Initializing Communications...");
}

void decodeMessage()
{
    /*
    This is where the message received from the NodeMCU 
    will be decoded to determine what to do.
    */
    switch(inBuffer[0])
    {
      case CHANGE_MODE:
        switch(inBuffer[1]) 
        {
          case HOME:        Serial.println("SmartMat: Changing mode to HOME...");
                            mode = HOME;
                            digitalWrite(BUZZER, LOW);
                            break;
          case AWAY:        Serial.println("SmartMat: Changing mode to AWAY...");
                            mode = AWAY;
                            digitalWrite(BUZZER, LOW);
                            break;
          case NIGHT:       Serial.println("SmartMat: Changing mode to NIGHT...");
                            mode = NIGHT;
                            digitalWrite(BUZZER, LOW);
                            break;
          case LOCKED:      Serial.println("SmartMat: Changing mode to LOCKED...");
                            mode = LOCKED;
                            digitalWrite(BUZZER, LOW);
                            break;
          default:          Serial.println("ERROR: Did not recognize mode to change to...");
                            break;                                                                                
        }
        break;
      case CHANGE_WEIGHT_MODE:
        switch(inBuffer[1])
        {
          case KILOGRAMS:   Serial.println("SmartMat: Changing weight mode to KILOGRAMS...");
                            weight_mode = KILOGRAMS;
                            break;
          case POUNDS:      Serial.println("SmartMat: Changing weight mode to POUNDS...");
                            weight_mode = POUNDS;
                            break;
          default:          Serial.println("ERROR: Did not recognize weight mode to change to...");
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


//****************************************************
//Main Loop                                          *
//****************************************************

void setup()
{  
  //Start the software serial for communication with the NodeMCU
  initializeCommunications();
  
  if (initializeWeightDetection() == EXIT_FAILURE)
  {
    Serial.println("SmartMat: Accurate Weight Detection Layer failed to initialize.");
    while(1);
  }
  /*if (initializeWeightDistribution() == EXIT_FAILURE)
  {
    Serial.println("SmartMat: Accurate Weight Distribution Layer failed to initialize.");
    while(1);
  }*/
  Serial.println("SmartMat: SmartMat Initialization Complete");
  Serial.print("\n");
  Serial.println("            SmartMat             ");
  Serial.println("*********************************");
  Serial.print("Mode: ");
  switch(mode) 
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
  switch(notification_mode) 
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
  //Determine Mode Handle
  if (mode_flag == ON)
  {
      mode_flag = OFF;
      switch(mode)
      {
        case HOME:        homeModeHandle();
                          break;
        case AWAY:        awayModeHandle();
                          break;
        case NIGHT:       nightModeHandle();
                          break;
        case LOCKED:      lockedModeHandle();
                          break;
        default:          homeModeHandle(); 
                          break;
      }
  }
  
  //Check for message from NodeMCU
  checkforMessage();
  if (uart_flag == 2)
  {
    decodeMessage();
  }

  //Timers
  //Check for Live Stream Timer
  if (liveStreamTimer.isReady())
  {
    //sendWeight();
    liveStreamTimer.reset();
  }
  //UART for Bad Messages
  if ((receiveTimer.isReady()) && (uart_flag == 1))
  {
    uart_flag = 0;
    buffer_len = 0;
    Serial.println("SmartMat: Dropping Message...");
  }
  if (weightChangeTimer.isReady())
  {
    float currentWeight = getWeight();
    if ((currentWeight >= weight + THRESHOLD))
    {
      Serial.println("SmartMat: Some weight has been detected...");
      weight = getWeight();
      mode_flag = ON;
    }
    else if ((currentWeight <= weight - THRESHOLD))
    {
      Serial.println("SmartMat: Some weight has been removed...");
      weight = getWeight();
      mode_flag = ON;
    }
    //sendWeight();
    weightChangeTimer.reset();
  }
  if (buzzerTimer.isReady())
  {
    digitalWrite(BUZZER, LOW);
  }

}
