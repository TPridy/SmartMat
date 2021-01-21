#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);

int data;
void setup() {
s.begin(9600);
Serial.begin(9600);
}
 
void loop() {
  delay(5000);
  s.write("0123456789");
  Serial.println("0123456789");
}
 
 
