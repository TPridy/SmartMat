/*
  settings.h - Library for settings to be used with SmartMat.
  Created by Thomas C Pridy, January 21st, 2021.
  Released into the public domain.
*/

#ifndef Settings_h
#define Settings_h
#include "Arduino.h"

enum Modes{
    NONE,
    STAYATHOME,
    AWAY,
    NIGHT,
    LOCKED,
    ALARM
};

enum Weight_Modes{
    KILOGRAMS,
    POUNDS
};

class Settings {

    //Private
    private:
        char mode;
        char weight_mode;

    //Public
    public:
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