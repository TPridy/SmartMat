#include <SoftwareSerial.h>
#include "settings.h"
#include "HX711.h"

//Settings
byte mode = HOME;
byte weight_mode = KILOGRAMS;
byte notifications = DONT_SEND;

//NodeMCU
SoftwareSerial NodeMCU(RX, TX); 

//Scale
HX711  scale;

//Flags
byte mode_flag = OFF;

//Accurate Weight Detection Variables
float weight = 0;

//Accurate Weight Distribution Variables    
int matrix[16][16];   //Value Matrix
int calibra[16][16];  //Calibration Matrix 
char inBuffer[64];
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


//****************************************************
//Secret Sauce                                       *
//****************************************************
void noneModeHandle()
{
  //De-activate buzzer
  digitalWrite(BUZZER, LOW);
  return;
}

void homeModeHandle()
{
  //De-activate buzzer
  digitalWrite(BUZZER, LOW);

  Serial.print("Weight ->");
  float new_weight = getWeight();
  Serial.println(weight);
  
  if (new_weight > weight + THRESHOLD)
  {
    Serial.println("Caught");
    mode = ALARM;
    mode_flag = ON;
  }
  else
  {
    weight = new_weight;
  }
  
  return;
}

void awayModeHandle()
{
  //De-activate buzzer
  digitalWrite(BUZZER, LOW);
  
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
  //De-activate buzzer
  digitalWrite(BUZZER, LOW);
  
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
  //De-activate buzzer
  digitalWrite(BUZZER, LOW);
  
  /*
  if (getWeight() < getLastReading() - THRESHOLD)
  {
    //Send Notifcation that the weight has left the mat
    //Change to ALARM mode
  }
  */
  return;
}

void alarmModeHandle()
{
  Serial.println("SmartMat: ALARM mode activated.");
  //Activate buzzer
  digitalWrite(BUZZER, HIGH);
  //Send test messages to neighbors
  return;
}

void checkWeightChange()
{
  float new_weight = getWeight();
  Serial.print("Weight->");
  Serial.println(new_weight);
  if ((new_weight > weight + THRESHOLD) || (new_weight < weight - THRESHOLD))
  {
    switch(mode)
    {
      case LOCKED:    mode = ALARM;
                      mode_flag = ON;
                      weight = new_weight;
                      break;
      default:        break;
    }
  }
  else
  {
    weight = new_weight;
  }
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
/*
float getLastReading()
{
    //Return weight
    //return last_reading;
}

void calibrateHX711(float calibration_factor)
{
    scale.set_scale(calibration_factor);
}*/

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

char detectionAlgorithm()
{
  int max_num1 = 0;
  int x1,y1 = 0;
  int max_num2 = 0;
  int x2,y2 = 0;
  
  for(int j = 0; j < 15; j++)
  { 
    for(int i = 0; i < 15; i++)
    {
      if ((matrix[j][i] > max_num1) && (matrix[j][i] > 0))
      {
        max_num1 = matrix[j][i];
        x1 = j;
        y1 = i;
      }
    } 
  }
  for(int j = 0; j < 15; j++)
  { 
    for(int i = 0; i < 15; i++)
    {
      if ((matrix[j][i] > max_num2) && (matrix[j][i] > 0) && (j != x1) && (i != y1))
      {
        if (((j >= x1 + 3) || (j <= x1 - 3)) && ((i >= y1 + 3) || (i <= y1 - 3)))
        {
          max_num2 = matrix[j][i];
          x2 = j;
          y2 = i;
        }
      }
    } 
  }
  Serial.println(max_num1);
  Serial.println(x1);
  Serial.println(y1);
  Serial.println(max_num2);
  Serial.println(x2);
  Serial.println(y2);

}

//****************************************************
//UART Communication                                 *
//****************************************************

void checkforMessage()
{ 
  if (NodeMCU.available() > 0)
  {
    inBuffer[0] = NodeMCU.read();
    byte done = OFF;
    byte i = 0;

    if(inBuffer[0] == '{')
    {
      Serial.println("SmartMat: Received a Message from NodeMCU");
      delay(1000);
      while((done == OFF) && (NodeMCU.available() > 0))
      {
        inBuffer[i+1] = NodeMCU.read();
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

void decodeMessage(char i)
{
    /*
    This is where the message received from the NodeMCU 
    will be decoded to determine what to do.
    */
    switch(inBuffer[1])
    {
      case CHANGE_MODE:
        switch(inBuffer[2]) 
        {
          case NONE:        Serial.println("SmartMat: Changing mode to NONE...");
                            mode = NONE;
                            mode_flag = ON;
                            break;
          case HOME:        Serial.println("SmartMat: Changing mode to HOME...");
                            mode = HOME;
                            mode_flag = ON;
                            break;
          case AWAY:        Serial.println("SmartMat: Changing mode to AWAY...");
                            mode = AWAY;
                            mode_flag = ON;
                            break;
          case NIGHT:       Serial.println("SmartMat: Changing mode to NIGHT...");
                            mode = NIGHT;
                            mode_flag = ON;
                            break;
          case LOCKED:      Serial.println("SmartMat: Changing mode to LOCKED...");
                            mode = LOCKED;
                            mode_flag = ON;
                            break;
          case ALARM:       Serial.println("SmartMat: Changing mode to ALARM...");
                            mode = ALARM;
                            mode_flag = ON;
                            break;
          default:          Serial.println("ERROR: Did not recognize mode to change to...");
                            break;                                                                                
        }
        break;
      case CHANGE_WEIGHT_MODE:
        switch(inBuffer[2])
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
      case NOTIFICATIONS:
        switch(inBuffer[2])
        {
          case LIVE_STREAM_WEIGHT:    Serial.println("SmartMat: Changing notification mode to LIVE_STREAM_WEIGHT...");
                                      notifications = LIVE_STREAM_WEIGHT;
                                      break;
          case SEND_NOTIFICATION:     Serial.println("SmartMat: Changing notification to SEND_NOTIFICATION...");
                                      notifications = SEND_NOTIFICATION;
                                      break;
          case SEND_EMAIL:            Serial.println("SmartMat: Changing notification to SEND_EMAIL...");
                                      notifications = SEND_EMAIL;
                                      break;
          case DONT_SEND:             Serial.println("SmartMat: Changing notification to DONT_SEND...");
                                      notifications = DONT_SEND;
                                      break;
          default:          Serial.println("ERROR: Did not recognize weight mode to change to...");
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


//****************************************************
//General                                            *
//****************************************************
void printBanner()
{
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
  switch(notifications) 
  {
    case LIVE_STREAM_WEIGHT:             Serial.println("LIVE STREAM");
                                         break;
    case SEND_NOTIFICATION:              Serial.println("TEXT");
                                         break;
    case SEND_EMAIL:                     Serial.println("EMAIL");
                                         break; 
    case DONT_SEND:                      Serial.println("NO MESSAGE");
                                         break;                                                                 
    default:          Serial.println("\nERROR: Did not recognize mode...");
                      break;                                                                                
  }
  Serial.print("\n");
}

//****************************************************
//Main Loop                                          *
//****************************************************

void setup()
{  
  //Start the software serial for communication with the NodeMCU
  initializeCommunications();
  printBanner();
  /*if (initializeWeightDetection() == EXIT_FAILURE)
  {
    Serial.println("SmartMat: Accurate Weight Detection Layer failed to initialize.");
    while(1);
  }
  if (initializeWeightDistribution() == EXIT_FAILURE)
  {
    Serial.println("SmartMat: Accurate Weight Distribution Layer failed to initialize.");
    while(1);
  }*/
  for(int q = 0; q < 15; q++)
  { 
    for(int w = 0; w < 15; w++)
    {
      matrix[q][w] = 0;
    } 
  }
  matrix[4][9] = 45;
  matrix[0][8] = 42;
  detectionAlgorithm();

}
 
void loop()
{
  //Determine Mode Handle
  if (mode_flag == ON)
  {
      mode_flag = OFF;
      switch(mode)
      {
        case NONE:        noneModeHandle();
                          break;
        case HOME:        homeModeHandle();
                          break;
        case AWAY:        awayModeHandle();
                          break;
        case NIGHT:       nightModeHandle();
                          break;
        case LOCKED:      lockedModeHandle();
                          break;
        case ALARM:       alarmModeHandle();
                          break;
        default:          noneModeHandle(); 
                          break;
      }
  }
  
  //Check for message from NodeMCU
  checkforMessage();

  /*char message[4];
  float test_weight;
  test_weight = 1134.23;
  switch(weight_mode)
  {
    case KILOGRAMS: test_weight = test_weight/1000;
                    break;
    case POUNDS:    test_weight = test_weight*0.00220462;
                    break;
    default:        break;
  }
  String string = String(test_weight);
  message[0] = '{';
  message[1] = NOTIFICATIONS;
  message[2] = SEND_NOTIFICATION;
  message[3] = PERSON;
  message[4] = string[0];
  message[5] = string[1];
  message[6] = string[2];
  message[7] = string[3];
  message[8] = string[4];
  message[9] = string[5];
  message[10] = string[6];
  message[11] = string[7];
  message[12] = '}';
  
  //strcpy(&message[3],&string,6);

  NodeMCU.write(message,13);

  //Check for Weight Change
  delay(10000);
  //checkWeightChange();*/
}
