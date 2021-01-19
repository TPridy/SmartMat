#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);

int data;
void setup() {
s.begin(115200);
Serial.begin(115200);
}
 
void loop() {
  delay(100);
  s.write("s");
  if (s.available()>0)
  {
    data=s.read();
    Serial.println(data);
  }
}
 
 
