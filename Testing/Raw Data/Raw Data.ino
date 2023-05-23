#include <HardwareSerial.h>

HardwareSerial gpsSerial(1);  // select which UART bridge to use, UART0 is used for USB data transfer

void setup(){
  Serial.begin(115200); // connect serial
  gpsSerial.begin(9600, SERIAL_8N1, 22, 21); // connect gps sensor
}


void loop(){
  while(gpsSerial.available()){
    Serial.write(gpsSerial.read());
  }
}