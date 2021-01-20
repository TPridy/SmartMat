/*
  settings.cpp - Library for settings to be used with SmartMat.
  Created by Thomas C Pridy, January 21st, 2021.
  Released into the public domain.
*/

#include "settings.h"
#include "Arduino.h"

Settings::Settings(char new_mode ,char new_weight_mode ){
    setMode(new_mode);
    setWeightMode(new_weight_mode);
}

void Settings::updateSettings(char new_mode, char new_weight_mode){
    setMode(new_mode);
    setWeightMode(new_weight_mode);

}

char Settings::getMode(){
    return mode;
}

char Settings::getWeightMode(){
    return weight_mode;
}

char Settings::getPhoneNumber(){
    return weight_mode;
}

void Settings::setMode(char new_mode){
    mode = new_mode;
}

void Settings::setWeightMode(char new_weight_mode){
    weight_mode = new_weight_mode;
}

