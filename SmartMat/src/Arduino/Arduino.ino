#include <SoftwareSerial.h>
#include "settings.h"
#include "HX711.h"

//Settings
byte mode = HOME;
byte weight_mode = POUNDS;

//NodeMCU
SoftwareSerial NodeMCU(7, 8); 

//Scale
HX711  scale;

//****************************************************
//Accurate Weight Distribution Settings and Pins     *
//****************************************************

int matrix[15][15];   //Value Matrix
int calibra[15][15];  //Calibration Matrix 

int inByte = 0;
int valor = 0;
int minsensor=254;

const boolean muxChannel[15][4]={
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
    //{1,1,1,1}  //channel 15
  };


//****************************************************
//Modes                                              *
//****************************************************
void noneModeHandle()
{
  
  return;
}

void HomeModeHandle()
{
  
  /*
  if (getWeight() > getLastReading() + THRESHOLD)
  {
    //Send Notifcation that there is weight on the mat
  }
  */
  return;
}

void awayModeHandle()
{
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
  //Serial.print("LOCKED\n");
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
  //Serial.print("ALARM\n");
  //Set timer so can only be in alarm mode for specified time
  //ACtivate buzzer
  //Send test messages to neighbors
  return;
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
    //1 second delay for Last Reading
    //delay(1000);
    
    //Get data
    float reading = scale.get_units();
    //Serial.println(reading,DEC);
    
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

//****************************************************
//UART Communication                                 *
//****************************************************

void checkforMessage()
{
  if (Serial.available() > 0)
  {
    inByte = Serial.read();
    
    if(inByte == 'A')
    {
      getAcuurateWeightDistributionMatrx();
      printAccurateWeightDistributionMatrix();
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
  NodeMCU.begin(115200); 
  Serial.print("SmartMat: Initializing Communications...\n");
}
/*
void decodeMessage(const char *message,char length,Settings *settings)
{
    /*
    This is where the message received from the NodeMCU 
    will be decoded to determine what to do.
    *//*
    switch(message[0])
    {
      case CHANGE_MODE:
        switch(message[1]) 
        {
          case NONE:        Serial.println("SmartMat: Changing mode to NONE...");
                            mode = NONE;
                            break;
          case HOME:  Serial.println("SmartMat: Changing mode to HOME...");
                            mode = HOME;
                            break;
          case AWAY:        Serial.println("SmartMat: Changing mode to AWAY...");
                            mode = AWAY;
                            break;
          case NIGHT:       Serial.println("SmartMat: Changing mode to NIGHT...");
                            mode = NIGHT;
                            break;
          case LOCKED:      Serial.println("SmartMat: Changing mode to LOCKED...");
                            mode = LOCKED;
                            break;
          case ALARM:       Serial.println("SmartMat: Changing mode to ALARM...");
                            mode = ALARM;
                            break;
          default:          Serial.println("ERROR: Did not recognize mode to change to...");
                            break;                                                                                
        }
        break;
      case CHANGE_WEIGHT_MODE:
        switch(message[1])
        {
          case KILOGRAMS:   Serial.println("SmartMat: Changing weight mode to KILOGRAMS...");
                            settings->setWeightMode(KILOGRAMS);
                            break;
          case POUNDS:      Serial.println("SmartMat: Changing weight mode to POUNDS...");
                            settings->setWeightMode(POUNDS);
                            break;
          default:          Serial.println("ERROR: Did not recognize weight mode to change to...");
                            break;                                                                                
        }
        break;
      default:  
        Serial.print("ERROR: Cannot decode the message -> ");
        Serial.println(message);
        break;
    }
    return;
}*/


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
  if (initializeWeightDistribution() == EXIT_FAILURE)
  {
    Serial.println("SmartMat: Accurate Weight Distribution Layer failed to initialize.");
    while(1);
  }
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
  Serial.print("\n");
}
 
void loop()
{
  switch(mode)
  {
    case NONE:        noneModeHandle();
                      break;
    case HOME:        HomeModeHandle();
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

  //Check for message from NodeMCU
  checkforMessage();
}
