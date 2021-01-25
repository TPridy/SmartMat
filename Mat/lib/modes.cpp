/*
  modes.cpp - Library for modes to be used with SmartMat.
  Created by Thomas C Pridy, January 21st, 2021.
  Released into the public domain.
*/

#include "modes.h"
#include "Arduino.h"
#include "pressure_sensors.h"

void noneModeHandle()
{
  getWeight();
  return;
}

void stayAtHomeModeHandle()
{
  //Serial.print("HOME\n");
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
