#include <SoftwareSerial.h>
#include "settings.h"
#include "modes.h"
#include "uart.h"
SoftwareSerial s(D6,D5);

char message[2];
char message2[2];
char message3[2];
int data;
void setup() {
s.begin(9600);
Serial.begin(9600);
message[0] = CHANGE_MODE;
message[1] = AWAY;
message2[0] = CHANGE_MODE;
message2[1] = LOCKED;
message3[0] = CHANGE_WEIGHT_MODE;
message3[1] = 100;

}
 
void loop() {
  Serial.println("NodeMCU: Changing mode to AWAY");
  s.write(message,2);
  Serial.println(message);
  delay(1000);
  Serial.println("NodeMCU: Changing mode to LOCKED");
  s.write(message2,2);
  Serial.println(message2);
  delay(1000);
  Serial.println("NodeMCU: Changing weight mode to KILOGRAMS");
  s.write(message3,2);
  Serial.println(message3);
  delay(1000);
  //s.write("0123456789");
  //Serial.println("0123456789");
}
 
 
