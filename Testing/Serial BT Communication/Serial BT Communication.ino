#include <SoftwareSerial.h> 
SoftwareSerial BTSerial(0, 1); // RX | TX 
int flag = 0; 
int LED = 8; 
void setup() 
{   
 Serial.begin(9600); 
 BTSerial.begin(9600);
 Serial.println("Ready to connect\nDefualt password is 1234 or 000"); 
} 
void loop() 
{ 
 if (BTSerial.available()) {
   Serial.write(BTSerial.read());
  }

}