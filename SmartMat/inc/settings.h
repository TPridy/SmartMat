/*
  settings.h - Library for settings to be used with SmartMat.
  Created by Thomas C Pridy, January 21st, 2021.
  Released into the public domain.
*/

#ifndef Settings_h
#define Settings_h
#include "Arduino.h"

//Modes
#define NONE 0
#define STAYATHOME 1
#define AWAY 2
#define NIGHT 3
#define LOCKED 4
#define ALARM 5

//Weight Modes
#define KILOGRAMS 1
#define POUNDS 2

//WiFi Status
#define DISCONNECTED 0
#define CONNECTED 1

class Settings {
        
    //Public
    public:
        //Variables
        char mode;
        char weight_mode;

        //Constructors
        Settings(char new_mode = NONE,char new_weight_mode = POUNDS);
        //Methods
        void updateSettings(char new_mode, char new_weight_mode);
        //Getters
        char getMode();
        char getWeightMode();
        char getPhoneNumber();
        //Setters
        void setMode(char new_mode); 
        void setWeightMode(char new_weight_mode);
      
};

#endif
