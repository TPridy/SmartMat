#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);

int data;
void setup() {
s.begin(9600);
Serial.begin(9600);
}
 
void loop() {
  delay(4000);
  s.write("Hello");
  Serial.println("Hello");
  
}
 
 
